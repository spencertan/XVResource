namespace Xivi
{
template <typename Base>
ResourceCache<Base>::~ResourceCache()
{
  delete m_fallback;
}

//****************************************************************************
// Resource Group
//****************************************************************************

template <typename Base>
u64 ResourceCache<Base>::Size() const
{
  return m_datas.size();
}

template <typename Base>
u64 ResourceCache<Base>::Timestamp() const
{
  return m_timestamp;
}

template <typename Base>
void ResourceCache<Base>::Free()
{
  for ( auto it = m_datas.begin(); it != m_datas.end();)
  {
    if ( it->second._lifetime == ResourceLifetime::Managed )
      it = m_datas.erase( it );
    else
      it++;
  }
}

template <typename Base>
void ResourceCache<Base>::Clear()
{
  m_datas.clear();
}

//****************************************************************************
// Resource Data
//****************************************************************************

template <typename Base>
void ResourceCache<Base>::Load( ResourceKey key )
{
  if ( m_loader )
    m_loader->Load( key );
}

template <typename Base>
void ResourceCache<Base>::Load( ResourceKey key, std::string_view str )
{
  if ( m_loader )
    m_loader->Load( key, str );
}

template <typename Base>
template <typename Specialised>
requires (std::derived_from<Specialised, Base>)
Resource<Base, Specialised> ResourceCache<Base>::Get( ResourceKey key )
{
  if ( auto it = m_datas.find( key ); it == m_datas.end() )
  {
    Set( key, nullptr, ResourceState::Loading, ResourceLifetime::Managed );
    Load( key );
  }
  return Resource<Base,Specialised>( this, key );
}

template <typename Base>
void ResourceCache<Base>::Set( const ResourceKey key, const Ptr<Base> data, const ResourceState state, const ResourceLifetime lifetime )
{
  auto it = m_datas.find( key );

  XV_CORE_ASSERT_MSG( ( data == nullptr ) == ( state == ResourceState::NotFound || state == ResourceState::Loading ), "[ResourceCache] Data is nullptr" );

  XV_CORE_ASSERT_MSG( it == m_datas.end() || it->second.m_state != ResourceState::Final, "[ResourceCache] Data is Final" );

  if ( it == m_datas.end() )
    m_datas.emplace( std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(data, state, lifetime) );
  else
  {
    delete it->second.m_data;

    it->second.m_data = data;
    it->second.m_state = state;
    it->second.m_lifetime = lifetime;
  }
  ++m_timestamp;
}

template <typename Base>
u64 ResourceCache<Base>::ReferenceCount( const ResourceKey key ) const
{
  const auto it = m_datas.find( key );
  if ( it == m_datas.end() )
    return 0;
  return it->second.m_referenceCount;
}

template <typename Base>
ResourceState ResourceCache<Base>::State( const ResourceKey key ) const
{
  const auto it = m_datas.find( key );

  if ( it == m_datas.end() )
  {
    if ( m_fallback )
      return ResourceState::NotLoadedFallback;
    else
      return ResourceState::NotLoaded;
  }

  if ( !it->second.m_data )
  {
    if ( m_fallback )
    {
      if ( it->second.m_state == ResourceState::Loading )
        return ResourceState::LoadingFallback;
      else if ( it->second.m_state == ResourceState::NotFound )
        return ResourceState::NotFoundFallback;
      else
        return ResourceState::NotLoadedFallback;
    }
    else
      return ResourceState::NotLoaded;
  }

  return it->second.m_state;
}

//****************************************************************************
// Fallback
//****************************************************************************
template <typename Base>
Ptr<Base> ResourceCache<Base>::Fallback()
{
  return m_fallback;
}

template <typename Base>
Ptr<const Base> ResourceCache<Base>::Fallback() const
{
  return m_fallback;
}

template <typename Base>
void ResourceCache<Base>::SetFallback( const Ptr<Base> data )
{
  delete m_fallback;
  m_fallback = data;
  ++m_timestamp;
}

//****************************************************************************
// Loaders
//****************************************************************************

template <typename Base>
Ptr<IResourceLoader<Base>> ResourceCache<Base>::Loader()
{
  return m_loader;
}

template <typename Base>
Ptr<const IResourceLoader<Base>> ResourceCache<Base>::Loader() const
{
  return m_loader;
}

template <typename Base>
void ResourceCache<Base>::SetLoader( const Ptr<IResourceLoader<Base>> loader )
{
  delete m_loader;

  if ( m_loader = loader; m_loader )
    m_loader->m_cache = this;
}

template <typename Base>
void ResourceCache<Base>::FreeLoader()
{
  if ( !m_loader )
    return;

  delete m_loader;
}

//****************************************************************************
// Reference Counting
//****************************************************************************

template <typename Base>
void ResourceCache<Base>::IncrementReference( ResourceKey key )
{
  auto it = m_datas.find( key );
  if ( it != m_datas.end() )
    it->second.m_count++;
}

template <typename Base>
void ResourceCache<Base>::DecrementReference( ResourceKey key )
{
  auto it = m_datas.find( key );
  if ( it != m_datas.end() )
    if ( --( it->second.m_count ) == 0 &&
         it->second.m_lifetime == ResourceLifetime::Referenced )
      m_datas.erase( it );
}

template <typename Base>
const std::vector<ResourceKey> ResourceCache<Base>::List() const
{
  std::vector<ResourceKey> vec;
  for ( auto it = m_datas.begin(); it != m_datas.end(); ++it )
    vec.push_back( it->first );
  return vec;
}

} // namespace Xivi