#pragma once

#include <Utils/Assert.h>

namespace Xivi::Resource
{
template <typename Base>
struct DataWrapper
{
  using ReferenceCount = u32;
  UniquePtr<Base> m_data { nullptr };
  std::string m_file {};
  ReferenceCount m_reference_count { 0 };
  Resource::State m_state { Resource::State::Final };
  Resource::Policy m_policy { Resource::Policy::Persistant };

  explicit DataWrapper() = default;
  explicit DataWrapper( const std::string& file, Ptr<Base> data, Resource::State state, Resource::Policy policy ) :
    m_file(file),
    m_data( data ),
    m_state( state ),
    m_policy( policy )
  {}

// No Copy / Move
  DataWrapper( const DataWrapper & ) = delete;
  DataWrapper &operator=( const DataWrapper & ) = delete;
  DataWrapper( DataWrapper &&rhs ) = delete;
  DataWrapper &operator=( DataWrapper && ) = delete;

  ~DataWrapper()
  {
    XV_CORE_ASSERT_MSG( m_count == 0, "[Data] Existing reference exists" );
  }
};
} // namespace Xivi