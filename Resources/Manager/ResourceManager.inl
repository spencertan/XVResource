namespace Xivi
{

template <typename... Ts>
ResourceManager<Ts...>::~ResourceManager()
{
  FreeLoaders<Ts...>();
}

template <typename... Ts>
template <typename Base>
u64 ResourceManager<Ts...>::Size() const
{
  return ResourceCache<Base>::Size();
}

template <typename... Ts>
template <typename Base>
u64 ResourceManager<Ts...>::ReferenceCount( ResourceKey key ) const
{
  return ResourceCache<Base>::ReferenceCount( key );
}

template <typename... Ts>
template <typename Base>
ResourceState ResourceManager<Ts...>::State( ResourceKey key ) const
{
  return ResourceCache<Base>::State( key );
}

template <typename... Ts>
template <typename Base>
ResourceManager<Ts...> &ResourceManager<Ts...>::Load( ResourceKey key )
{
  ResourceCache<Base>::Load( key );
  return *this;
}

template <typename... Ts>
template <typename Base>
ResourceManager<Ts...> &ResourceManager<Ts...>::Load( ResourceKey key, std::string_view str )
{
  ResourceCache<Base>::Load( key, str );
  return *this;
}

template <typename... Ts>
template <typename Base, typename Specialised>
requires (std::derived_from<Specialised, Base>)
Resource<Base, Specialised> ResourceManager<Ts...>::Get( ResourceKey key )
{
  return ResourceCache<Base>::template Get<Specialised>( key );
}

template <typename... Ts>
template <typename Base>
ResourceManager<Ts...> &ResourceManager<Ts...>::Set( ResourceKey key, Ptr<Base> data, ResourceState state, ResourceLifetime lifetime )
{
  ResourceCache<Base>::Set( key, data, state, lifetime );
  return *this;
}

template <typename... Ts>
template <typename Base>
ResourceManager<Ts...> &ResourceManager<Ts...>::Set( ResourceKey key, Base &&data, ResourceState state, ResourceLifetime lifetime )
{
  return Set( key, new Raw<Base>( std::forward<Base>( data ) ), state, lifetime );
}

template <typename... Ts>
template <typename Base>
ResourceManager<Ts...> &ResourceManager<Ts...>::Set( ResourceKey key, Ptr<Base> data )
{
  return Set( key, data, ResourceState::Final, ResourceLifetime::Persistant );
}

template <typename... Ts>
template <typename Base>
ResourceManager<Ts...> &ResourceManager<Ts...>::Set( ResourceKey key, Base &&data )
{
  return Set( key, new Raw<Base>( std::forward<Base>( data ) ) );
}

template <typename... Ts>
template <typename Base>
ResourceManager<Ts...> &ResourceManager<Ts...>::Free()
{
  ResourceCache<Base>::Free();
  return *this;
}

template <typename... Ts>
ResourceManager<Ts...> &ResourceManager<Ts...>::Free()
{
  FreeInternal<Ts...>();
  return *this;
}

template <typename... Ts>
template <typename Base>
ResourceManager<Ts...> &ResourceManager<Ts...>::Clear()
{
  ResourceCache<Base>::Clear();
  return *this;
}

template <typename... Ts>
ResourceManager<Ts...> &ResourceManager<Ts...>::Clear()
{
  ClearInternal<Ts...>();
  return *this;
}

template <typename... Ts>
template <typename Base>
Ptr<Base> ResourceManager<Ts...>::Fallback()
{
  return ResourceCache<Base>::Fallback();
}

template <typename... Ts>
template <typename Base>
Ptr<const Base> ResourceManager<Ts...>::Fallback() const
{
  return ResourceCache<Base>::Fallback();
}

template <typename... Ts>
template <typename Base>
ResourceManager<Ts...> &ResourceManager<Ts...>::SetFallback( Ptr<Base> data )
{
  ResourceCache<Base>::SetFallback( data );
  return *this;
}

template <typename... Ts>
template <typename Base>
ResourceManager<Ts...> &ResourceManager<Ts...>::SetFallback( Base &&data )
{
  return SetFallback( new Raw<Base>( std::forward<Base>( data ) ) );
}

template <typename... Ts>
template <typename Base>
Ptr<IResourceLoader<Base>> ResourceManager<Ts...>::Loader()
{
  return ResourceCache<Base>::Loader();
}

template <typename... Ts>
template <typename Base>
Ptr<const IResourceLoader<Base>> ResourceManager<Ts...>::Loader() const
{
  return ResourceCache<Base>::Loader();
}

template <typename... Ts>
template <typename Base>
ResourceManager<Ts...> &ResourceManager<Ts...>::SetLoader( Ptr<IResourceLoader<Base>> loader )
{
  ResourceCache<Base>::SetLoader( loader );
  return *this;
}

template <typename... Ts>
template <typename Base>
const std::vector<ResourceKey> ResourceManager<Ts...>::List() const
{
  return ResourceCache<Base>::List();
}

template <typename... Ts>
template <typename... Resources>
void ResourceManager<Ts...>::FreeInternal()
{
  ( Free<Resources>(), ... );
}

template <typename... Ts>
template <typename... Resources>
void ResourceManager<Ts...>::ClearInternal()
{
  ( Clear<Resources>(), ... );
}

template <typename... Ts>
template <typename... Resources>
void ResourceManager<Ts...>::FreeLoaders()
{
  (ResourceCache<Resources>::FreeLoader(),...);
}

} // namespace Xivi