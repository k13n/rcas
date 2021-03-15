#include "cas/partition_importer.hpp"
#include "cas/binary_key.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <deque>
#include <stdexcept>
#include <cstring>


template<class VType>
cas::PartitionImporter<VType>::PartitionImporter(cas::Cas<VType>& index)
    : index_(index)
{ }


template<class VType>
uint64_t cas::PartitionImporter<VType>::BulkLoad(
    const std::string& filename,
    size_t dataset_size,
    size_t page_size)
{
  std::deque<cas::BinaryKey> keys;

  std::vector<uint8_t> buffer;
  buffer.resize(page_size);
  std::FILE* infile = fopen(filename.c_str(), "rb");
  if (infile == nullptr) {
    throw std::runtime_error{"file cannot be opened: " + filename};
  }

  size_t total_bytes_read = 0;
  while (total_bytes_read < dataset_size) {
    size_t bytes_read = fread(&buffer[0], 1, page_size, infile);
    if (bytes_read < page_size) {
      break;
    }

    uint16_t offset = 0;
    uint16_t nr_keys;
    CopyFromPage(&buffer[0], page_size, offset, &nr_keys, sizeof(nr_keys));

    for (uint16_t i = 0; i < nr_keys; ++i) {
      cas::BinaryKey key;
      uint16_t len_path = 0;
      uint16_t len_value = 0;
      CopyFromPage(&buffer[0], page_size, offset, &len_path, sizeof(len_path));
      CopyFromPage(&buffer[0], page_size, offset, &len_value, sizeof(len_value));
      key.path_.resize(len_path);
      key.value_.resize(len_value);
      CopyFromPage(&buffer[0], page_size, offset, &key.ref_[0], 20);
      CopyFromPage(&buffer[0], page_size, offset, &key.path_[0], len_path);
      CopyFromPage(&buffer[0], page_size, offset, &key.value_[0], len_value);
      keys.push_back(key);
    }

    total_bytes_read += bytes_read;
  }
  fclose(infile);

  return index_.BulkLoad(keys);
}


template<class VType>
void cas::PartitionImporter<VType>::CopyFromPage(
    uint8_t* page,
    size_t page_size,
    uint16_t& offset,
    void* dst,
    size_t count)
{
  if (offset + count > page_size) {
    auto msg = "page address violation: " + std::to_string(offset+count);
    throw std::out_of_range{msg};
  }
  std::memcpy(dst, page+offset, count);
  offset += count;
}


// explicit instantiations to separate header from implementation
template class cas::PartitionImporter<cas::vint32_t>;
template class cas::PartitionImporter<cas::vint64_t>;
template class cas::PartitionImporter<cas::vstring_t>;
