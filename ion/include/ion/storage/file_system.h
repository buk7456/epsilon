#ifndef ION_STORAGE_H
#define ION_STORAGE_H

#include <assert.h>
#include <ion/unicode/utf8_helper.h>
#include <omg/global_box.h>

#include "record.h"
#include "record_name_verifier.h"
#include "storage_delegate.h"
#include "storage_helper.h"

namespace Ion {

// clang-format off
/* Storage : | Magic |             Record1                 |            Record2                  | ... | Magic |
 *           | Magic | Size1(uint16_t) | FullName1 | Body1 | Size2(uint16_t) | FullName2 | Body2 | ... | Magic |
 *
 * A record's fullName is baseName.extension. */
// clang-format on

namespace Storage {

class FileSystem {
  friend class Record;
  friend OMG::GlobalBox<FileSystem>;

 public:
  typedef uint16_t record_size_t;

  constexpr static size_t k_storageSize = 42 * 1024;
  static_assert(UINT16_MAX >= k_storageSize - 1,
                "record_size_t not big enough");

  static OMG::GlobalBox<FileSystem> sharedFileSystem;

#if ION_STORAGE_LOG
  void log();
#endif

  size_t availableSize();
  size_t putAvailableSpaceAtEndOfRecord(Record r);
  void getAvailableSpaceFromEndOfRecord(Record r, size_t recordAvailableSpace);
  uint32_t checksum();

  // Storage delegate
  void setDelegate(StorageDelegate *delegate) { m_delegate = delegate; }
  void notifyChangeToDelegate(const Record r = Record()) const;
  Record::ErrorStatus notifyFullnessToDelegate() const;

  // Record name verifier
  RecordNameVerifier *recordNameVerifier() { return &m_recordNameVerifier; }

  // Record counters
  int numberOfRecordsWithExtension(const char *extension) {
    return numberOfRecordsWithFilter(extension, ExtensionOnlyFilter);
  }
  // NOTE: We could handle the "hidden" status at Record level instead of this
  int numberOfRecordsStartingWithout(const char nonStartingChar,
                                     const char *extension) {
    return numberOfRecordsWithFilter(extension, FirstCharFilter,
                                     &nonStartingChar);
  }

  // Record names helper
  typedef bool (*IsNameAvailableTest)(char *buffer, size_t bufferSize,
                                      void *auxiliary);
  int firstAvailableNameFromPrefix(char *buffer, size_t prefixLength,
                                   size_t bufferSize, IsNameAvailableTest test,
                                   void *auxiliary = nullptr, int maxId = 99);
  int firstAvailableNameFromPrefix(char *buffer, size_t prefixLength,
                                   size_t bufferSize, const char *extension,
                                   int maxId = 99);

  // Record creation
  Record::ErrorStatus createRecordWithExtension(
      const char *baseName, const char *extension, const void *data,
      size_t size, bool extensionCanOverrideItself = false);
  Record::ErrorStatus createRecordWithFullNameAndDataChunks(
      const char *fullName, const void *dataChunks[], size_t sizeChunks[],
      size_t numberOfChunks, bool extensionCanOverrideItself = false);
  Record::ErrorStatus createRecordWithDataChunks(
      Record::Name recordName, const void *dataChunks[], size_t sizeChunks[],
      size_t numberOfChunks, bool extensionCanOverrideItself = false);

  template <typename T>
  void initSystemRecord() {
    assert(!hasRecord(
        Ion::Storage::Record(T::k_recordName, Ion::Storage::systemExtension)));
    T object;
    createRecordWithExtension(T::k_recordName, Ion::Storage::systemExtension,
                              &object, sizeof(object));
    assert(hasRecord(
        Ion::Storage::Record(T::k_recordName, Ion::Storage::systemExtension)));
  }

  // Record getters
  bool hasRecord(Record r) { return pointerOfRecord(r) != nullptr; }
  Record recordWithExtensionAtIndex(const char *extension, int index) {
    return recordWithFilterAtIndex(extension, index, ExtensionOnlyFilter);
  }
  Record recordWithExtensionAtIndexStartingWithout(const char nonStartingChar,
                                                   const char *extension,
                                                   int index) {
    return recordWithFilterAtIndex(extension, index, FirstCharFilter,
                                   &nonStartingChar);
  }
  Record recordNamed(Record::Name name);
  Record recordNamed(const char *fullName) {
    return recordNamed(Record::CreateRecordNameFromFullName(fullName));
  }
  Record recordBaseNamedWithExtension(const char *baseName,
                                      const char *extension) {
    return recordNamed(
        Record::CreateRecordNameFromBaseNameAndExtension(baseName, extension));
  }
  Record recordBaseNamedWithExtensions(const char *baseName,
                                       const char *const extensions[],
                                       size_t numberOfExtensions);
  const char *extensionOfRecordBaseNamedWithExtensions(
      const char *baseName, int baseNameLength, const char *const extensions[],
      size_t numberOfExtensions);

  template <typename T>
  T *findSystemRecord() {
    Ion::Storage::Record record = recordBaseNamedWithExtension(
        T::k_recordName, Ion::Storage::systemExtension);
    assert(!record.isNull());
    Ion::Storage::Record::Data data = record.value();
    assert(data.size == sizeof(T));
    return static_cast<T *>(const_cast<void *>(data.buffer));
  }

  // Record destruction
  void destroyAllRecords();
  void destroyRecordsWithExtension(const char *extension);

  /* Destroy competing records.
   * Return true if other records with same full name or competing names
   * have been destroyed.
   * Return false if other records with same full name or competing names
   * still exist but can't be destroyed.
   * (see RecordNameVerifier for more info on competing names)
.*/
  bool handleCompetingRecord(Record::Name recordName,
                             bool destroyRecordWithSameFullName,
                             bool notifyDelegate = true);

 private:
  constexpr static uint32_t Magic = 0xEE0BDDBA;
  constexpr static size_t k_maxRecordSize = (1 << sizeof(record_size_t) * 8);

  // Record filter on names
  typedef bool (*RecordFilter)(Record::Name name, const void *auxiliary);
  static bool ExtensionOnlyFilter(Record::Name name, const void *auxiliary) {
    return true;
  };
  static bool FirstCharFilter(Record::Name name, const void *auxiliary) {
    assert(auxiliary);
    return name.baseName[0] != *static_cast<const char *>(auxiliary);
  };
  // Private record counters and getters
  int numberOfRecordsWithFilter(const char *extension, RecordFilter filter,
                                const void *auxiliary = nullptr);
  Record recordWithFilterAtIndex(const char *extension, int index,
                                 RecordFilter filter,
                                 const void *auxiliary = nullptr);
  void destroyRecordsMatching(RecordFilter filter, const void *auxiliary);

  FileSystem();

  /* Getters/Setters on recordID */
  Record::Name nameOfRecord(const Record record) const;
  Record::ErrorStatus setNameOfRecord(Record *record, Record::Name name);
  Record::Data valueOfRecord(const Record record);
  Record::ErrorStatus setValueOfRecord(const Record record, Record::Data data);
  bool destroyRecord(const Record record, bool notifyDelegate = true);

  /* Getters on address in buffer */
  char *pointerOfRecord(const Record record) const;
  record_size_t sizeOfRecordStarting(char *start) const;
  const void *valueOfRecordStarting(char *start) const;
  Record::Name nameOfRecordStarting(char *start) const;

  /* Overriders */
  size_t overrideSizeAtPosition(char *position, record_size_t size);
  size_t overrideNameAtPosition(char *position, Record::Name name);
  size_t overrideValueAtPosition(char *position, const void *data,
                                 record_size_t size);

  bool isNameOfRecordTaken(Record r, const Record *recordToExclude = nullptr);
  char *endBuffer();
  size_t sizeOfRecordWithName(Record::Name name, size_t dataSize);
  bool slideBuffer(char *position, int delta);
  class RecordIterator {
   public:
    RecordIterator(char *start) : m_recordStart(start) {}
    char *operator*() { return m_recordStart; }
    RecordIterator &operator++();
    bool operator!=(const RecordIterator &it) const {
      return m_recordStart != it.m_recordStart;
    }

   private:
    char *m_recordStart;
  };
  RecordIterator begin() const {
    if (sizeOfRecordStarting((char *)m_buffer) == 0) {
      return nullptr;
    }
    return RecordIterator((char *)m_buffer);
  };
  RecordIterator end() const { return RecordIterator(nullptr); }

  Record privateRecordBasedNamedWithExtensions(
      const char *baseName, int baseNameLength, const char *const extensions[],
      size_t numberOfExtensions, const char **extensionResult = nullptr);
  bool recordNameHasBaseNameAndOneOfTheseExtensions(
      Record::Name name, const char *baseName, int baseNameLength,
      const char *const extensions[], size_t numberOfExtensions,
      const char **extensionResult);

  uint32_t m_magicHeader;
  char m_buffer[k_storageSize];
  uint32_t m_magicFooter;
  StorageDelegate *m_delegate;
  RecordNameVerifier m_recordNameVerifier;
  mutable Record m_lastRecordRetrieved;
  mutable char *m_lastRecordRetrievedPointer;
};

}  // namespace Storage

}  // namespace Ion

#endif
