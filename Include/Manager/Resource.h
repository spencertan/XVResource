#pragma once
#include <type_traits>
#include "ResourceKey.h"
#include "ResourceEnum.h"
#include "ResourceCache.h"

namespace Xivi
{

template <typename Base, typename Specialised = Base>
class Resource
{
public:
  // Default
  explicit Resource() = default;

  // Copy
  Resource( const Resource &rhs );
  Resource &operator=( const Resource &rhs );

  // Move
  Resource( Resource &&rhs );
  Resource &operator=( Resource &&rhs );

  // Destructor
  ~Resource();

  bool operator==( const Resource &rhs ) const;
  bool operator!=( const Resource &rhs ) const;
  operator bool();
  operator Specialised *( );
  Specialised &operator*();
  Specialised *operator->();

  const ResourceKey &Key() const;
  const ResourceState &State();

  // RTTR
  void SetKey( const ResourceKey &key );
  const ResourceKey &GetKey() const;

private:
  friend ResourceCache<Base>;

  Resource( Ptr<ResourceCache<Base>> group, ResourceKey key );
  void Sync();

  ResourceKey m_key {};
  Ptr<ResourceCache<Base>> m_cache { nullptr };
  Ptr<Base> m_data { nullptr };
  u64 m_timestamp { 0 };
  ResourceState m_state { ResourceState::Final };
};

}

#include "Resource.inl"