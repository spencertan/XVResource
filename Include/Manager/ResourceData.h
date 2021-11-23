#pragma once

#include <Utils/Assert.h>

namespace Xivi
{
template <typename Base>
struct ResourceData
{
  using ReferenceCount = u32;
  Ptr<Base> m_data { nullptr };
  ReferenceCount m_count { 0 };
  ResourceState m_state { ResourceState::Final };
  ResourceLifetime m_lifetime { ResourceLifetime::Persistant };

  explicit ResourceData() = default;
  explicit ResourceData( Ptr<Base> data, ResourceState state, ResourceLifetime lifetime ) :
    m_data( data ),
    m_state( state ),
    m_lifetime( lifetime )
  {}

// No Copy / Move
  ResourceData( const ResourceData & ) = delete;
  ResourceData &operator=( const ResourceData & ) = delete;
  ResourceData( ResourceData &&rhs ) = delete;
  ResourceData &operator=( ResourceData && ) = delete;

  ~ResourceData()
  {
    XV_CORE_ASSERT_MSG( m_count == 0, "[ResourceData] Existing reference exists" );
    delete m_data;
  }
};
} // namespace Xivi