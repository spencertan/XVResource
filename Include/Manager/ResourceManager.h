#pragma once

#include <Types/Base.h>
#include "Resource.h"
#include "Loader/IResourceLoader.h"

namespace Xivi
{
  template <typename... Ts>
  class ResourceManager : private ResourceCache<Ts>...
  {
    template <typename... Resources>
    using ResourceList = Typelist<Resources...>;

  public:
    explicit ResourceManager() = default;

    ~ResourceManager();

    // ResourceCache
    template <typename Base>
    u64 Size() const;

    template <typename Base>
    u64 ReferenceCount(ResourceKey key) const;

    template <typename Base>
    ResourceState State(ResourceKey key) const;

    // Resource
    template <typename Base>
    ResourceManager<Ts...> &Load( ResourceKey key );

    template <typename Base>
    ResourceManager<Ts...> &Load( ResourceKey key, std::string_view str );

    template <typename Base, typename Specialised = Base>
    requires (std::derived_from<Specialised, Base>)
    Resource<Base, Specialised> Get(ResourceKey key);

    template <typename Base>
    ResourceManager<Ts...> &Set(ResourceKey key, Ptr<Base> data, ResourceState state, ResourceLifetime lifetime);

    template <typename Base>
    ResourceManager<Ts...> &Set(ResourceKey key, Base &&data, ResourceState state, ResourceLifetime lifetime);

    template <typename Base>
    ResourceManager<Ts...> &Set(ResourceKey key, Ptr<Base> data);

    template <typename Base>
    ResourceManager<Ts...> &Set(ResourceKey key, Base &&data);

    template <typename Base>
    ResourceManager<Ts...> &Free();

    ResourceManager<Ts...> &Free();

    template <typename Base>
    ResourceManager<Ts...> &Clear();

    ResourceManager<Ts...> &Clear();

    // Fallbacks
    template <typename Base>
    Ptr<Base> Fallback();

    template <typename Base>
    Ptr<const Base> Fallback() const;

    template <typename Base>
    ResourceManager<Ts...> &SetFallback(Ptr<Base> data);

    template <typename Base>
    ResourceManager<Ts...> &SetFallback(Base &&data);

    // Loaders
    template <typename Base>
    Ptr<IResourceLoader<Base>> Loader();

    template <typename Base>
    Ptr<const IResourceLoader<Base>> Loader() const;

    template <typename Base>
    ResourceManager<Ts...> &SetLoader(Ptr<IResourceLoader<Base>> loader);

    template <typename Base>
    const std::vector<ResourceKey> List() const;

  private:
    template <typename... Resources>
    void FreeInternal();

    template <typename... Resources>
    void ClearInternal();

    template <typename... Resources>
    void FreeLoaders();
  };
} // namespace Xivi

#include "ResourceManager.inl"

#include <Types/Traits/Specialised.h>
namespace Xivi::Concepts
{
template <typename Base>
concept IsResourceType = Xivi::Traits::IsSpecialised<Resource, Base>();
}