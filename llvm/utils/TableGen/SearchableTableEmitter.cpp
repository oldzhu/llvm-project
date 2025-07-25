//===- SearchableTableEmitter.cpp - Generate efficiently searchable tables -==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This tablegen backend emits a generic array initialized by specified fields,
// together with companion index tables and lookup functions. The lookup
// function generated is either a direct lookup (when a single primary key field
// is integral and densely numbered) or a binary search otherwise.
//
//===----------------------------------------------------------------------===//

#include "Basic/CodeGenIntrinsics.h"
#include "Common/CodeGenTarget.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/MapVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/TableGen/Error.h"
#include "llvm/TableGen/Record.h"
#include "llvm/TableGen/TableGenBackend.h"
#include <set>
#include <string>
#include <vector>

using namespace llvm;

#define DEBUG_TYPE "searchable-table-emitter"

static int64_t getAsInt(const Init *B) {
  if (const auto *BI = dyn_cast<BitsInit>(B))
    return *BI->convertInitializerToInt();
  if (const auto *II = dyn_cast<IntInit>(B))
    return II->getValue();
  llvm_unreachable("Unexpected initializer");
}

static int64_t getInt(const Record *R, StringRef Field) {
  return getAsInt(R->getValueInit(Field));
}

namespace {
struct GenericEnum {
  struct Entry {
    StringRef Name;
    int64_t Value;
    Entry(StringRef N, int64_t V) : Name(N), Value(V) {}
  };

  std::string Name;
  const Record *Class = nullptr;
  std::string PreprocessorGuard;
  MapVector<const Record *, Entry> Entries;

  const Entry *getEntry(const Record *Def) const {
    auto II = Entries.find(Def);
    if (II == Entries.end())
      return nullptr;
    return &II->second;
  }
};

struct GenericField {
  std::string Name;
  const RecTy *RecType = nullptr;
  bool IsCode = false;
  bool IsIntrinsic = false;
  bool IsInstruction = false;
  GenericEnum *Enum = nullptr;

  GenericField(StringRef Name) : Name(Name.str()) {}
};

struct SearchIndex {
  std::string Name;
  SMLoc Loc; // Source location of PrimaryKey or Key field definition.
  SmallVector<GenericField, 1> Fields;
  bool EarlyOut = false;
  bool ReturnRange = false;
};

struct GenericTable {
  std::string Name;
  ArrayRef<SMLoc> Locs; // Source locations from the Record instance.
  std::string PreprocessorGuard;
  std::string CppTypeName;
  SmallVector<GenericField, 2> Fields;
  std::vector<const Record *> Entries;

  std::unique_ptr<SearchIndex> PrimaryKey;
  SmallVector<std::unique_ptr<SearchIndex>, 2> Indices;

  const GenericField *getFieldByName(StringRef Name) const {
    for (const auto &Field : Fields) {
      if (Name == Field.Name)
        return &Field;
    }
    return nullptr;
  }
};

class SearchableTableEmitter {
  const RecordKeeper &Records;
  std::unique_ptr<CodeGenTarget> Target;
  std::vector<std::unique_ptr<GenericEnum>> Enums;
  DenseMap<const Record *, GenericEnum *> EnumMap;
  std::set<std::string> PreprocessorGuards;

public:
  explicit SearchableTableEmitter(const RecordKeeper &R) : Records(R) {}

  void run(raw_ostream &OS);

private:
  typedef std::pair<const Init *, int> SearchTableEntry;

  enum TypeContext {
    TypeInStaticStruct,
    TypeInTempStruct,
    TypeInArgument,
  };

  std::string primaryRepresentation(SMLoc Loc, const GenericField &Field,
                                    const Init *I) {
    if (const auto *SI = dyn_cast<StringInit>(I)) {
      if (Field.IsCode || SI->hasCodeFormat())
        return SI->getValue().str();
      else
        return SI->getAsString();
    }
    if (const auto *BI = dyn_cast<BitsInit>(I))
      return "0x" + utohexstr(getAsInt(BI));
    if (const auto *BI = dyn_cast<BitInit>(I))
      return BI->getValue() ? "true" : "false";
    if (Field.IsIntrinsic)
      return "Intrinsic::" + getIntrinsic(I).EnumName.str();
    if (Field.IsInstruction)
      return I->getAsString();
    if (Field.Enum) {
      const GenericEnum::Entry *Entry =
          Field.Enum->getEntry(cast<DefInit>(I)->getDef());
      if (!Entry)
        PrintFatalError(Loc,
                        Twine("Entry for field '") + Field.Name + "' is null");
      return Entry->Name.str();
    }
    PrintFatalError(Loc, Twine("invalid field type for field '") + Field.Name +
                             "'; expected: bit, bits, string, or code");
  }

  bool isIntrinsic(const Init *I) {
    if (const auto *DI = dyn_cast<DefInit>(I))
      return DI->getDef()->isSubClassOf("Intrinsic");
    return false;
  }

  const CodeGenIntrinsic &getIntrinsic(const Init *I) {
    const Record *Def = cast<DefInit>(I)->getDef();
    return Target->getIntrinsic(Def);
  }

  bool compareBy(const Record *LHS, const Record *RHS,
                 const SearchIndex &Index);

  std::string searchableFieldType(const GenericTable &Table,
                                  const SearchIndex &Index,
                                  const GenericField &Field, TypeContext Ctx) {
    if (isa<StringRecTy>(Field.RecType)) {
      if (Ctx == TypeInStaticStruct)
        return "const char *";
      if (Ctx == TypeInTempStruct)
        return "std::string";
      return "StringRef";
    }
    if (const auto *BI = dyn_cast<BitsRecTy>(Field.RecType)) {
      unsigned NumBits = BI->getNumBits();
      if (NumBits <= 8)
        return "uint8_t";
      if (NumBits <= 16)
        return "uint16_t";
      if (NumBits <= 32)
        return "uint32_t";
      if (NumBits <= 64)
        return "uint64_t";
      PrintFatalError(Index.Loc, Twine("In table '") + Table.Name +
                                     "' lookup method '" + Index.Name +
                                     "', key field '" + Field.Name +
                                     "' of type bits is too large");
    }
    if (isa<BitRecTy>(Field.RecType))
      return "bool";
    if (Field.Enum || Field.IsIntrinsic || Field.IsInstruction)
      return "unsigned";
    PrintFatalError(Index.Loc,
                    Twine("In table '") + Table.Name + "' lookup method '" +
                        Index.Name + "', key field '" + Field.Name +
                        "' has invalid type: " + Field.RecType->getAsString());
  }

  void emitGenericTable(const GenericTable &Table, raw_ostream &OS);
  void emitGenericEnum(const GenericEnum &Enum, raw_ostream &OS);
  void emitLookupDeclaration(const GenericTable &Table,
                             const SearchIndex &Index, raw_ostream &OS);
  void emitLookupFunction(const GenericTable &Table, const SearchIndex &Index,
                          bool IsPrimary, raw_ostream &OS);
  void emitIfdef(StringRef Guard, raw_ostream &OS);

  bool parseFieldType(GenericField &Field, const Init *II);
  std::unique_ptr<SearchIndex>
  parseSearchIndex(GenericTable &Table, const RecordVal *RecVal, StringRef Name,
                   ArrayRef<StringRef> Key, bool EarlyOut, bool ReturnRange);
  void collectEnumEntries(GenericEnum &Enum, StringRef NameField,
                          StringRef ValueField, ArrayRef<const Record *> Items);
  void collectTableEntries(GenericTable &Table, ArrayRef<const Record *> Items);
  int64_t getNumericKey(const SearchIndex &Index, const Record *Rec);
};

} // End anonymous namespace.

// For search indices that consists of a single field whose numeric value is
// known, return that numeric value.
int64_t SearchableTableEmitter::getNumericKey(const SearchIndex &Index,
                                              const Record *Rec) {
  assert(Index.Fields.size() == 1);
  const GenericField &Field = Index.Fields[0];

  // To be consistent with compareBy and primaryRepresentation elsewhere,
  // we check for IsInstruction before Enum-- these fields are not exclusive.
  if (Field.IsInstruction) {
    const Record *TheDef = Rec->getValueAsDef(Field.Name);
    return Target->getInstrIntValue(TheDef);
  }
  if (Field.Enum) {
    const Record *EnumEntry = Rec->getValueAsDef(Field.Name);
    return Field.Enum->getEntry(EnumEntry)->Value;
  }
  assert(isa<BitsRecTy>(Field.RecType) && "unexpected field type");

  return getInt(Rec, Field.Name);
}

/// Less-than style comparison between \p LHS and \p RHS according to the
/// key of \p Index.
bool SearchableTableEmitter::compareBy(const Record *LHS, const Record *RHS,
                                       const SearchIndex &Index) {
  // Compare two values and return:
  // * -1 if LHS < RHS.
  // *  1 if LHS > RHS.
  // *  0 if LHS == RHS.
  auto CmpLTValue = [](const auto &LHS, const auto &RHS) -> int {
    if (LHS < RHS)
      return -1;
    if (LHS > RHS)
      return 1;
    return 0;
  };

  // Specialized form of `CmpLTValue` for string-like types that uses compare()
  // to do the comparison of the 2 strings once (instead if 2 comparisons if we
  // use `CmpLTValue`).
  auto CmpLTString = [](StringRef LHS, StringRef RHS) -> int {
    return LHS.compare(RHS);
  };

  // Compare two fields and returns:
  // - true if LHS < RHS.
  // - false if  LHS > RHS.
  // - std::nullopt if LHS == RHS.
  auto CmpLTField = [this, &Index, &CmpLTValue,
                     &CmpLTString](const Init *LHSI, const Init *RHSI,
                                   const GenericField &Field) -> int {
    if (isa<BitsRecTy>(Field.RecType) || isa<IntRecTy>(Field.RecType)) {
      int64_t LHSi = getAsInt(LHSI);
      int64_t RHSi = getAsInt(RHSI);
      return CmpLTValue(LHSi, RHSi);
    }

    if (Field.IsIntrinsic) {
      const CodeGenIntrinsic &LHSi = getIntrinsic(LHSI);
      const CodeGenIntrinsic &RHSi = getIntrinsic(RHSI);
      if (int Cmp = CmpLTString(LHSi.TargetPrefix, RHSi.TargetPrefix))
        return Cmp;
      return CmpLTString(LHSi.Name, RHSi.Name);
    }

    if (Field.IsInstruction) {
      // This does not correctly compare the predefined instructions!
      const Record *LHSr = cast<DefInit>(LHSI)->getDef();
      const Record *RHSr = cast<DefInit>(RHSI)->getDef();

      // Order pseudo instructions before non-pseudo ones.
      bool LHSNotPseudo = !LHSr->getValueAsBit("isPseudo");
      bool RHSNotPseudo = !RHSr->getValueAsBit("isPseudo");
      if (int Cmp = CmpLTValue(LHSNotPseudo, RHSNotPseudo))
        return Cmp;
      return CmpLTString(LHSr->getName(), RHSr->getName());
    }

    if (Field.Enum) {
      const Record *LHSr = cast<DefInit>(LHSI)->getDef();
      const Record *RHSr = cast<DefInit>(RHSI)->getDef();
      int64_t LHSv = Field.Enum->getEntry(LHSr)->Value;
      int64_t RHSv = Field.Enum->getEntry(RHSr)->Value;
      return CmpLTValue(LHSv, RHSv);
    }

    std::string LHSs = primaryRepresentation(Index.Loc, Field, LHSI);
    std::string RHSs = primaryRepresentation(Index.Loc, Field, RHSI);
    if (isa<StringRecTy>(Field.RecType)) {
      LHSs = StringRef(LHSs).upper();
      RHSs = StringRef(RHSs).upper();
    }
    return CmpLTString(LHSs, RHSs);
  };

  for (const GenericField &Field : Index.Fields) {
    const Init *LHSI = LHS->getValueInit(Field.Name);
    const Init *RHSI = RHS->getValueInit(Field.Name);
    if (int Cmp = CmpLTField(LHSI, RHSI, Field))
      return Cmp < 0;
  }
  return false;
}

void SearchableTableEmitter::emitIfdef(StringRef Guard, raw_ostream &OS) {
  OS << "#ifdef " << Guard << "\n";
  PreprocessorGuards.insert(Guard.str());
}

/// Emit a generic enum.
void SearchableTableEmitter::emitGenericEnum(const GenericEnum &Enum,
                                             raw_ostream &OS) {
  emitIfdef((Twine("GET_") + Enum.PreprocessorGuard + "_DECL").str(), OS);

  OS << "enum " << Enum.Name << " {\n";
  for (const auto &[Name, Value] :
       make_second_range(Enum.Entries.getArrayRef()))
    OS << "  " << Name << " = " << Value << ",\n";
  OS << "};\n";

  OS << "#endif\n\n";
}

void SearchableTableEmitter::emitLookupFunction(const GenericTable &Table,
                                                const SearchIndex &Index,
                                                bool IsPrimary,
                                                raw_ostream &OS) {
  OS << "\n";
  emitLookupDeclaration(Table, Index, OS);
  OS << " {\n";

  std::vector<const Record *> IndexRowsStorage;
  ArrayRef<const Record *> IndexRows;
  StringRef IndexTypeName;
  StringRef IndexName;

  if (IsPrimary) {
    IndexTypeName = Table.CppTypeName;
    IndexName = Table.Name;
    IndexRows = Table.Entries;
  } else {
    OS << "  struct IndexType {\n";
    for (const auto &Field : Index.Fields) {
      OS << "    "
         << searchableFieldType(Table, Index, Field, TypeInStaticStruct) << " "
         << Field.Name << ";\n";
    }
    OS << "    unsigned _index;\n";
    OS << "  };\n";

    OS << "  static const struct IndexType Index[] = {\n";

    std::vector<std::pair<const Record *, unsigned>> Entries;
    Entries.reserve(Table.Entries.size());
    for (auto [Idx, TblEntry] : enumerate(Table.Entries))
      Entries.emplace_back(TblEntry, Idx);

    llvm::stable_sort(Entries,
                      [&](const std::pair<const Record *, unsigned> &LHS,
                          const std::pair<const Record *, unsigned> &RHS) {
                        return compareBy(LHS.first, RHS.first, Index);
                      });

    IndexRowsStorage.reserve(Entries.size());
    for (const auto &[EntryRec, EntryIndex] : Entries) {
      IndexRowsStorage.push_back(EntryRec);

      OS << "    { ";
      ListSeparator LS;
      for (const auto &Field : Index.Fields) {
        std::string Repr = primaryRepresentation(
            Index.Loc, Field, EntryRec->getValueInit(Field.Name));
        if (isa<StringRecTy>(Field.RecType))
          Repr = StringRef(Repr).upper();
        OS << LS << Repr;
      }
      OS << ", " << EntryIndex << " },\n";
    }

    OS << "  };\n\n";

    IndexTypeName = "IndexType";
    IndexName = "Index";
    IndexRows = IndexRowsStorage;
  }

  bool IsContiguous = false;

  if (Index.Fields.size() == 1 &&
      (Index.Fields[0].Enum || isa<BitsRecTy>(Index.Fields[0].RecType) ||
       Index.Fields[0].IsInstruction)) {
    int64_t FirstKeyVal = getNumericKey(Index, IndexRows[0]);
    IsContiguous = true;
    for (const auto &[Idx, IndexRow] : enumerate(IndexRows)) {
      if (getNumericKey(Index, IndexRow) != FirstKeyVal + (int64_t)Idx) {
        IsContiguous = false;
        break;
      }
    }
  }

  if (Index.EarlyOut || IsContiguous) {
    const GenericField &Field = Index.Fields[0];
    std::string FirstRepr = primaryRepresentation(
        Index.Loc, Field, IndexRows[0]->getValueInit(Field.Name));
    std::string LastRepr = primaryRepresentation(
        Index.Loc, Field, IndexRows.back()->getValueInit(Field.Name));
    std::string TS =
        '(' + searchableFieldType(Table, Index, Field, TypeInStaticStruct) +
        ')';
    OS << "  if (" << TS << Field.Name << " != std::clamp(" << TS << Field.Name
       << ", " << TS << FirstRepr << ", " << TS << LastRepr << "))\n";
    OS << "    return nullptr;\n\n";

    if (IsContiguous && !Index.EarlyOut) {
      OS << "  auto Table = ArrayRef(" << IndexName << ");\n";
      OS << "  size_t Idx = " << Field.Name << " - " << FirstRepr << ";\n";
      OS << "  return ";
      if (IsPrimary)
        OS << "&Table[Idx]";
      else
        OS << "&" << Table.Name << "[Table[Idx]._index]";
      OS << ";\n";
      OS << "}\n";
      return;
    }
  }

  OS << "  struct KeyType {\n";
  for (const auto &Field : Index.Fields) {
    OS << "    " << searchableFieldType(Table, Index, Field, TypeInTempStruct)
       << " " << Field.Name << ";\n";
  }
  OS << "  };\n";
  OS << "  KeyType Key = {";
  ListSeparator LS;
  for (const auto &Field : Index.Fields) {
    OS << LS << Field.Name;
    if (isa<StringRecTy>(Field.RecType)) {
      OS << ".upper()";
      if (IsPrimary)
        PrintFatalError(Index.Loc,
                        Twine("In table '") + Table.Name +
                            "', use a secondary lookup method for "
                            "case-insensitive comparison of field '" +
                            Field.Name + "'");
    }
  }
  OS << "};\n";

  OS << "  struct Comp {\n";
  OS << "    bool operator()(const " << IndexTypeName
     << " &LHS, const KeyType &RHS) const {\n";

  auto emitComparator = [&]() {
    for (const auto &Field : Index.Fields) {
      if (isa<StringRecTy>(Field.RecType)) {
        OS << "      int Cmp" << Field.Name << " = StringRef(LHS." << Field.Name
           << ").compare(RHS." << Field.Name << ");\n";
        OS << "      if (Cmp" << Field.Name << " < 0) return true;\n";
        OS << "      if (Cmp" << Field.Name << " > 0) return false;\n";
      } else if (Field.Enum) {
        // Explicitly cast to unsigned, because the signedness of enums is
        // compiler-dependent.
        OS << "      if ((unsigned)LHS." << Field.Name << " < (unsigned)RHS."
           << Field.Name << ")\n";
        OS << "        return true;\n";
        OS << "      if ((unsigned)LHS." << Field.Name << " > (unsigned)RHS."
           << Field.Name << ")\n";
        OS << "        return false;\n";
      } else {
        OS << "      if (LHS." << Field.Name << " < RHS." << Field.Name
           << ")\n";
        OS << "        return true;\n";
        OS << "      if (LHS." << Field.Name << " > RHS." << Field.Name
           << ")\n";
        OS << "        return false;\n";
      }
    }
    OS << "      return false;\n";
    OS << "    }\n";
  };
  emitComparator();
  bool ShouldReturnRange = Index.ReturnRange;
  if (ShouldReturnRange) {
    OS << "    bool operator()(const KeyType &LHS, const " << IndexTypeName
       << " &RHS) const {\n";
    emitComparator();
  }

  OS << "  };\n";
  OS << "  auto Table = ArrayRef(" << IndexName << ");\n";
  if (ShouldReturnRange)
    OS << "  auto It = std::equal_range(Table.begin(), Table.end(), Key, ";
  else
    OS << "  auto Idx = std::lower_bound(Table.begin(), Table.end(), Key, ";
  OS << "Comp());\n";

  if (!ShouldReturnRange) {
    OS << "  if (Idx == Table.end()";
    for (const auto &Field : Index.Fields)
      OS << " ||\n      Key." << Field.Name << " != Idx->" << Field.Name;
  }

  if (ShouldReturnRange) {
    OS << "  return llvm::make_range(It.first, It.second);\n";
  } else if (IsPrimary) {
    OS << ")\n    return nullptr;\n\n";
    OS << "  return &*Idx;\n";
  } else {
    OS << ")\n    return nullptr;\n\n";
    OS << "  return &" << Table.Name << "[Idx->_index];\n";
  }

  OS << "}\n";
}

void SearchableTableEmitter::emitLookupDeclaration(const GenericTable &Table,
                                                   const SearchIndex &Index,
                                                   raw_ostream &OS) {
  if (Index.ReturnRange)
    OS << "llvm::iterator_range<const " << Table.CppTypeName << " *> ";
  else
    OS << "const " << Table.CppTypeName << " *";
  OS << Index.Name << "(";
  ListSeparator LS;
  for (const auto &Field : Index.Fields)
    OS << LS << searchableFieldType(Table, Index, Field, TypeInArgument) << " "
       << Field.Name;
  OS << ")";
}

void SearchableTableEmitter::emitGenericTable(const GenericTable &Table,
                                              raw_ostream &OS) {
  emitIfdef((Twine("GET_") + Table.PreprocessorGuard + "_DECL").str(), OS);

  // Emit the declarations for the functions that will perform lookup.
  if (Table.PrimaryKey) {
    emitLookupDeclaration(Table, *Table.PrimaryKey, OS);
    OS << ";\n";
  }
  for (const auto &Index : Table.Indices) {
    emitLookupDeclaration(Table, *Index, OS);
    OS << ";\n";
  }

  OS << "#endif\n\n";

  emitIfdef((Twine("GET_") + Table.PreprocessorGuard + "_IMPL").str(), OS);

  // The primary data table contains all the fields defined for this map.
  OS << "constexpr " << Table.CppTypeName << " " << Table.Name << "[] = {\n";
  for (const auto &[Idx, Entry] : enumerate(Table.Entries)) {
    OS << "  { ";

    ListSeparator LS;
    for (const auto &Field : Table.Fields)
      OS << LS
         << primaryRepresentation(Table.Locs[0], Field,
                                  Entry->getValueInit(Field.Name));

    OS << " }, // " << Idx << "\n";
  }
  OS << " };\n";

  // Indexes are sorted "{ Thing, PrimaryIdx }" arrays, so that a binary
  // search can be performed by "Thing".
  if (Table.PrimaryKey)
    emitLookupFunction(Table, *Table.PrimaryKey, /*IsPrimary=*/true, OS);
  for (const auto &Index : Table.Indices)
    emitLookupFunction(Table, *Index, /*IsPrimary=*/false, OS);

  OS << "#endif\n\n";
}

bool SearchableTableEmitter::parseFieldType(GenericField &Field,
                                            const Init *TypeOf) {
  auto Type = dyn_cast<StringInit>(TypeOf);
  if (!Type)
    return false;

  StringRef TypeStr = Type->getValue();

  if (TypeStr == "code") {
    Field.IsCode = true;
    return true;
  }

  if (const Record *TypeRec = Records.getDef(TypeStr)) {
    if (TypeRec->isSubClassOf("GenericEnum")) {
      Field.Enum = EnumMap[TypeRec];
      Field.RecType = RecordRecTy::get(Field.Enum->Class);
      return true;
    }
  }

  return false;
}

std::unique_ptr<SearchIndex> SearchableTableEmitter::parseSearchIndex(
    GenericTable &Table, const RecordVal *KeyRecVal, StringRef Name,
    ArrayRef<StringRef> Key, bool EarlyOut, bool ReturnRange) {
  auto Index = std::make_unique<SearchIndex>();
  Index->Name = Name.str();
  Index->Loc = KeyRecVal->getLoc();
  Index->EarlyOut = EarlyOut;
  Index->ReturnRange = ReturnRange;

  for (const auto &FieldName : Key) {
    const GenericField *Field = Table.getFieldByName(FieldName);
    if (!Field)
      PrintFatalError(
          KeyRecVal,
          Twine("In table '") + Table.Name +
              "', 'PrimaryKey' or 'Key' refers to nonexistent field '" +
              FieldName + "'");

    Index->Fields.push_back(*Field);
  }

  if (EarlyOut && isa<StringRecTy>(Index->Fields[0].RecType)) {
    PrintFatalError(
        KeyRecVal, Twine("In lookup method '") + Name + "', early-out is not " +
                       "supported for a first key field of type string");
  }

  return Index;
}

void SearchableTableEmitter::collectEnumEntries(
    GenericEnum &Enum, StringRef NameField, StringRef ValueField,
    ArrayRef<const Record *> Items) {
  Enum.Entries.reserve(Items.size());
  for (const Record *EntryRec : Items) {
    StringRef Name = NameField.empty() ? EntryRec->getName()
                                       : EntryRec->getValueAsString(NameField);
    int64_t Value = ValueField.empty() ? 0 : getInt(EntryRec, ValueField);
    Enum.Entries.try_emplace(EntryRec, Name, Value);
  }

  // If no values are provided for enums, assign values in the order of sorted
  // enum names.
  if (ValueField.empty()) {
    // Copy the map entries for sorting and clear the map.
    auto SavedEntries = Enum.Entries.takeVector();
    using MapVectorEntryTy = std::pair<const Record *, GenericEnum::Entry>;
    llvm::stable_sort(SavedEntries, [](const MapVectorEntryTy &LHS,
                                       const MapVectorEntryTy &RHS) {
      return LHS.second.Name < RHS.second.Name;
    });

    // Repopulate entries using the new sorted order.
    for (auto [Idx, Entry] : enumerate(SavedEntries))
      Enum.Entries.try_emplace(Entry.first, Entry.second.Name, Idx);
  }
}

void SearchableTableEmitter::collectTableEntries(
    GenericTable &Table, ArrayRef<const Record *> Items) {
  if (Items.empty())
    PrintFatalError(Table.Locs,
                    Twine("Table '") + Table.Name + "' has no entries");

  for (auto *EntryRec : Items) {
    for (auto &Field : Table.Fields) {
      auto TI = dyn_cast<TypedInit>(EntryRec->getValueInit(Field.Name));
      if (!TI || !TI->isComplete()) {
        PrintFatalError(EntryRec, Twine("Record '") + EntryRec->getName() +
                                      "' for table '" + Table.Name +
                                      "' is missing field '" + Field.Name +
                                      "'");
      }
      if (!Field.RecType) {
        Field.RecType = TI->getType();
      } else {
        const RecTy *Ty = resolveTypes(Field.RecType, TI->getType());
        if (!Ty)
          PrintFatalError(EntryRec->getValue(Field.Name),
                          Twine("Field '") + Field.Name + "' of table '" +
                              Table.Name + "' entry has incompatible type: " +
                              TI->getType()->getAsString() + " vs. " +
                              Field.RecType->getAsString());
        Field.RecType = Ty;
      }
    }

    Table.Entries.push_back(EntryRec); // Add record to table's record list.
  }

  const Record *IntrinsicClass = Records.getClass("Intrinsic");
  const Record *InstructionClass = Records.getClass("Instruction");
  for (auto &Field : Table.Fields) {
    if (!Field.RecType)
      PrintFatalError(Twine("Cannot determine type of field '") + Field.Name +
                      "' in table '" + Table.Name + "'. Maybe it is not used?");

    if (auto RecordTy = dyn_cast<RecordRecTy>(Field.RecType)) {
      if (IntrinsicClass && RecordTy->isSubClassOf(IntrinsicClass))
        Field.IsIntrinsic = true;
      else if (InstructionClass && RecordTy->isSubClassOf(InstructionClass))
        Field.IsInstruction = true;
    }
  }

  SearchIndex Idx;
  std::copy(Table.Fields.begin(), Table.Fields.end(),
            std::back_inserter(Idx.Fields));
  llvm::sort(Table.Entries, [&](const Record *LHS, const Record *RHS) {
    return compareBy(LHS, RHS, Idx);
  });
}

void SearchableTableEmitter::run(raw_ostream &OS) {
  // Emit tables in a deterministic order to avoid needless rebuilds.
  SmallVector<std::unique_ptr<GenericTable>, 4> Tables;
  DenseMap<const Record *, GenericTable *> TableMap;
  bool NeedsTarget =
      !Records.getAllDerivedDefinitionsIfDefined("Instruction").empty() ||
      !Records.getAllDerivedDefinitionsIfDefined("Intrinsic").empty();
  if (NeedsTarget)
    Target = std::make_unique<CodeGenTarget>(Records);

  // Collect all definitions first.
  for (const auto *EnumRec : Records.getAllDerivedDefinitions("GenericEnum")) {
    StringRef NameField;
    if (!EnumRec->isValueUnset("NameField"))
      NameField = EnumRec->getValueAsString("NameField");

    StringRef ValueField;
    if (!EnumRec->isValueUnset("ValueField"))
      ValueField = EnumRec->getValueAsString("ValueField");

    auto Enum = std::make_unique<GenericEnum>();
    Enum->Name = EnumRec->getName().str();
    Enum->PreprocessorGuard = EnumRec->getName().str();

    StringRef FilterClass = EnumRec->getValueAsString("FilterClass");
    Enum->Class = Records.getClass(FilterClass);
    if (!Enum->Class)
      PrintFatalError(EnumRec->getValue("FilterClass"),
                      Twine("Enum FilterClass '") + FilterClass +
                          "' does not exist");

    collectEnumEntries(*Enum, NameField, ValueField,
                       Records.getAllDerivedDefinitions(FilterClass));
    EnumMap.try_emplace(EnumRec, Enum.get());
    Enums.emplace_back(std::move(Enum));
  }

  for (const auto *TableRec :
       Records.getAllDerivedDefinitions("GenericTable")) {
    auto Table = std::make_unique<GenericTable>();
    Table->Name = TableRec->getName().str();
    Table->Locs = TableRec->getLoc();
    Table->PreprocessorGuard = TableRec->getName().str();
    Table->CppTypeName = TableRec->getValueAsString("CppTypeName").str();

    std::vector<StringRef> Fields = TableRec->getValueAsListOfStrings("Fields");
    for (const auto &FieldName : Fields) {
      Table->Fields.emplace_back(FieldName); // Construct a GenericField.

      if (auto TypeOfRecordVal =
              TableRec->getValue(("TypeOf_" + FieldName).str())) {
        if (!parseFieldType(Table->Fields.back(),
                            TypeOfRecordVal->getValue())) {
          PrintError(TypeOfRecordVal,
                     Twine("Table '") + Table->Name + "' has invalid 'TypeOf_" +
                         FieldName +
                         "': " + TypeOfRecordVal->getValue()->getAsString());
          PrintFatalNote("The 'TypeOf_xxx' field must be a string naming a "
                         "GenericEnum record, or \"code\"");
        }
      }
    }

    StringRef FilterClass = TableRec->getValueAsString("FilterClass");
    if (!Records.getClass(FilterClass))
      PrintFatalError(TableRec->getValue("FilterClass"),
                      Twine("Table FilterClass '") + FilterClass +
                          "' does not exist");

    const RecordVal *FilterClassFieldVal =
        TableRec->getValue("FilterClassField");
    std::vector<const Record *> Definitions =
        Records.getAllDerivedDefinitions(FilterClass);
    if (auto *FilterClassFieldInit =
            dyn_cast<StringInit>(FilterClassFieldVal->getValue())) {
      StringRef FilterClassField = FilterClassFieldInit->getValue();
      llvm::erase_if(Definitions, [&](const Record *R) {
        const RecordVal *Filter = R->getValue(FilterClassField);
        if (auto *BitV = dyn_cast<BitInit>(Filter->getValue()))
          return !BitV->getValue();

        PrintFatalError(Filter, Twine("FilterClassField '") + FilterClass +
                                    "' should be a bit value");
        return true;
      });
    }
    collectTableEntries(*Table, Definitions);

    if (!TableRec->isValueUnset("PrimaryKey")) {
      Table->PrimaryKey =
          parseSearchIndex(*Table, TableRec->getValue("PrimaryKey"),
                           TableRec->getValueAsString("PrimaryKeyName"),
                           TableRec->getValueAsListOfStrings("PrimaryKey"),
                           TableRec->getValueAsBit("PrimaryKeyEarlyOut"),
                           TableRec->getValueAsBit("PrimaryKeyReturnRange"));

      llvm::stable_sort(Table->Entries,
                        [&](const Record *LHS, const Record *RHS) {
                          return compareBy(LHS, RHS, *Table->PrimaryKey);
                        });
    }

    TableMap.try_emplace(TableRec, Table.get());
    Tables.emplace_back(std::move(Table));
  }

  for (const Record *IndexRec :
       Records.getAllDerivedDefinitions("SearchIndex")) {
    const Record *TableRec = IndexRec->getValueAsDef("Table");
    auto It = TableMap.find(TableRec);
    if (It == TableMap.end())
      PrintFatalError(IndexRec->getValue("Table"),
                      Twine("SearchIndex '") + IndexRec->getName() +
                          "' refers to nonexistent table '" +
                          TableRec->getName());

    GenericTable &Table = *It->second;
    Table.Indices.push_back(parseSearchIndex(
        Table, IndexRec->getValue("Key"), IndexRec->getName(),
        IndexRec->getValueAsListOfStrings("Key"),
        IndexRec->getValueAsBit("EarlyOut"), /*ReturnRange*/ false));
  }

  // Translate legacy tables.
  const Record *SearchableTable = Records.getClass("SearchableTable");
  for (auto &NameRec : Records.getClasses()) {
    const Record *Class = NameRec.second.get();
    if (Class->getDirectSuperClasses().size() != 1 ||
        !Class->isSubClassOf(SearchableTable))
      continue;

    StringRef TableName = Class->getName();
    ArrayRef<const Record *> Items =
        Records.getAllDerivedDefinitions(TableName);
    if (!Class->isValueUnset("EnumNameField")) {
      StringRef NameField = Class->getValueAsString("EnumNameField");
      StringRef ValueField;
      if (!Class->isValueUnset("EnumValueField"))
        ValueField = Class->getValueAsString("EnumValueField");

      auto Enum = std::make_unique<GenericEnum>();
      Enum->Name = (Twine(Class->getName()) + "Values").str();
      Enum->PreprocessorGuard = Class->getName().upper();
      Enum->Class = Class;

      collectEnumEntries(*Enum, NameField, ValueField, Items);

      Enums.emplace_back(std::move(Enum));
    }

    auto Table = std::make_unique<GenericTable>();
    Table->Name = (Twine(Class->getName()) + "sList").str();
    Table->Locs = Class->getLoc();
    Table->PreprocessorGuard = Class->getName().upper();
    Table->CppTypeName = Class->getName().str();

    for (const RecordVal &Field : Class->getValues()) {
      std::string FieldName = Field.getName().str();

      // Skip uninteresting fields: either special to us, or injected
      // template parameters (if they contain a ':').
      if (FieldName.find(':') != std::string::npos ||
          FieldName == "SearchableFields" || FieldName == "EnumNameField" ||
          FieldName == "EnumValueField")
        continue;

      Table->Fields.emplace_back(FieldName);
    }

    collectTableEntries(*Table, Items);

    for (const auto &Field :
         Class->getValueAsListOfStrings("SearchableFields")) {
      std::string Name =
          (Twine("lookup") + Table->CppTypeName + "By" + Field).str();
      Table->Indices.push_back(
          parseSearchIndex(*Table, Class->getValue(Field), Name, {Field},
                           /*EarlyOut*/ false, /*ReturnRange*/ false));
    }

    Tables.emplace_back(std::move(Table));
  }

  // Emit everything.
  for (const auto &Enum : Enums)
    emitGenericEnum(*Enum, OS);

  for (const auto &Table : Tables)
    emitGenericTable(*Table, OS);

  // Put all #undefs last, to allow multiple sections guarded by the same
  // define.
  for (const auto &Guard : PreprocessorGuards)
    OS << "#undef " << Guard << "\n";
}

static TableGen::Emitter::OptClass<SearchableTableEmitter>
    X("gen-searchable-tables", "Generate generic binary-searchable table");
