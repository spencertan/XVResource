#pragma once

#include "../ResourceCache.h"

namespace Xivi
{
  template <typename Base>
  class IResourceLoader
  {
    friend class ResourceCache<Base>;

    Ptr<ResourceCache<Base>> m_cache{nullptr};
    u64 m_requests{0};
    u64 m_loaded{0};
    u64 m_notFound{0};

  public:
    explicit IResourceLoader() = default;

    virtual ~IResourceLoader();

    u64 RequestCount() const;
    u64 LoadedCount() const;
    u64 NotFoundCount() const;
    
    void Load(ResourceKey key);
    void Load( ResourceKey key, std::string_view str );

  protected:
    void Set(ResourceKey key, Ptr<Base> data, ResourceState state, ResourceLifetime lifetime);

    template <typename Specialised>
    requires(!std::is_null_pointer_v<Raw<Specialised>>)
    void Set(ResourceKey key, Specialised &&data, ResourceState state, ResourceLifetime lifetime);

    void Set(ResourceKey key, Ptr<Base> data);

    template <typename Specialised>
    requires(!std::is_null_pointer_v<Raw<Specialised>>)
    void Set(ResourceKey key, Specialised &&data);

    void SetNotFound(ResourceKey key);
    
    virtual void DoLoad(ResourceKey key) = 0;
    virtual void DoLoad( ResourceKey key, std::string_view str ) = 0;
  };
} // namespace Xivi

#include "IResourceLoader.inl"