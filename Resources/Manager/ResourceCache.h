#pragma once
#include "ResourceData.h"
#include <unordered_map> 

namespace Xivi
{

template <typename>
class IResourceLoader;

class ResourceKey;

template <typename, typename>
class Resource;

template <typename Base>
class ResourceCache
{
  template <typename, typename>
  friend class Resource;

  friend class IResourceLoader<Base>;

  void IncrementReference( ResourceKey key );
  void DecrementReference( ResourceKey key );

  std::unordered_map<ResourceKey, ResourceData<Base>> m_datas;
  Ptr<Base> m_fallback { nullptr };
  Ptr<IResourceLoader<Base>> m_loader { nullptr };
  u64 m_timestamp { 0 };

protected:
  ResourceCache() = default;

public:

  // Disable copy
  ResourceCache( const ResourceCache & ) = delete;
  ResourceCache &operator=( const ResourceCache & ) = delete;

  // Disable move
  ResourceCache( ResourceCache && ) = delete;
  ResourceCache &operator=( ResourceCache && ) = delete;

  // Destructor
  virtual ~ResourceCache();

  // Resource group
  u64 Size() const;
  u64 Timestamp() const;
  void Free();
  void Clear();

  // Resource data
  void Load( ResourceKey key );
  void Load( ResourceKey key, std::string_view str );

  template <typename Specialised>
  requires (std::derived_from<Specialised, Base>)
  Resource<Base, Specialised> Get( ResourceKey key );

  void Set( ResourceKey key, Ptr<Base> data, ResourceState state, ResourceLifetime lifetime );
  u64 ReferenceCount( ResourceKey key ) const;
  ResourceState State( ResourceKey key ) const;

  // Fallback
  Ptr<Base> Fallback();
  Ptr<const Base> Fallback() const;
  void SetFallback( Ptr<Base> data );

  // Loaders
  Ptr<IResourceLoader<Base>> Loader();
  Ptr<const IResourceLoader<Base>> Loader() const;
  void SetLoader( Ptr<IResourceLoader<Base>> loader );
  void FreeLoader();

  const std::vector<ResourceKey> List() const;
};

} // namespace Xivi

#include "ResourceCache.inl"