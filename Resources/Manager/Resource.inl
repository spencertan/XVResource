namespace Xivi
{
  //*******************************************************************
  // Class Resource
  //*******************************************************************

  // Copy Constructor
template <typename Base, typename Specialised>
Resource<Base, Specialised>::Resource( const Resource &rhs ) :
  m_key { rhs.m_key },
  m_cache { rhs.m_cache },
  m_data { rhs.m_data },
  m_timestamp { rhs.m_timestamp },
  m_state { rhs.m_state }
{
  if ( m_cache )
    m_cache->IncrementReference( m_key );
}

// Copy Assignment
template <typename Base, typename Specialised>
Resource<Base, Specialised> &Resource<Base, Specialised>::operator=( const Resource &rhs )
{
  if ( m_cache )
    m_cache->DecrementReference( m_key );

  m_cache = rhs.m_cache;
  m_data = rhs.m_data;
  m_key = rhs.m_key;
  m_timestamp = rhs.m_timestamp;
  m_state = rhs.m_state;

  if ( m_cache )
    m_cache->IncrementReference( m_key );

  return *this;
}

// Move Constructor
template <typename Base, typename Specialised>
Resource<Base, Specialised>::Resource( Resource &&rhs ) :
  m_key { rhs.m_key },
  m_cache { rhs.m_cache },
  m_data { rhs.m_data },
  m_timestamp { rhs.m_timestamp },
  m_state { rhs.m_state }
{
  rhs.m_cache = nullptr;
  rhs.m_data = nullptr;
  rhs.m_key = {};
  rhs.m_timestamp = 0;
  rhs.m_state = ResourceState::Final;
}

// Move Assignment
template <typename Base, typename Specialised>
Resource<Base, Specialised> &Resource<Base, Specialised>::operator=( Resource &&rhs )
{
  std::swap( m_cache, rhs.m_cache );
  std::swap( m_data, rhs.m_data );
  std::swap( m_key, rhs.m_key );
  std::swap( m_timestamp, rhs.m_timestamp );
  std::swap( m_state, rhs.m_state );
  return *this;
}

// Destructor
template <typename Base, typename Specialised>
Resource<Base, Specialised>::~Resource()
{
  if ( m_cache )
    m_cache->DecrementReference( m_key );
}

template <typename Base, typename Specialised>
bool Resource<Base, Specialised>::operator==( const Resource &rhs ) const
{
  return m_cache == rhs.m_cache && m_key == rhs.m_key;
}

template <typename Base, typename Specialised>
bool Resource<Base, Specialised>::operator!=( const Resource &rhs ) const
{
  return !operator==( rhs );
}
template <typename Base, typename Specialised>
Resource<Base, Specialised>::operator bool()
{
  Sync();
  return m_data;
}

template <typename Base, typename Specialised>
Resource<Base, Specialised>::operator Specialised *( )
{
  Sync();
  return static_cast<Ptr<Specialised>>( m_data );
}

template <typename Base, typename Specialised>
Specialised &Resource<Base, Specialised>::operator*()
{
  Sync();
  return *static_cast<Ptr<Specialised>>( m_data );
}

template <typename Base, typename Specialised>
Specialised *Resource<Base, Specialised>::operator->()
{
  Sync();
  return static_cast<Ptr<Specialised>>( m_data );
}

template <typename Base, typename Specialised>
const ResourceKey &Resource<Base, Specialised>::Key() const
{
  return m_key;
}

template <typename Base, typename Specialised>
const ResourceState &Resource<Base, Specialised>::State()
{
  Sync();
  return m_state;
}

// Private Constructor
template <typename Base, typename Specialised>
Resource<Base, Specialised>::Resource( Ptr<ResourceCache<Base>> group, ResourceKey key ) :
  m_cache { group },
  m_data { nullptr },
  m_key { key },
  m_timestamp { 0 },
  m_state { ResourceState::NotLoaded }
{
  if ( m_cache )
    m_cache->IncrementReference( m_key );
}

// Private Internal Sync Resource
template <typename Base, typename Specialised>
void Resource<Base, Specialised>::Sync()
{
  if ( m_state == ResourceState::Final )
    return;

  if ( m_cache->m_timestamp <= m_timestamp )
    return;

  m_timestamp = m_cache->m_timestamp;

  const ResourceData<Base> &d = m_cache->m_datas[m_key];
  m_data = d.m_data;
  m_state = d.m_state;

  if ( !m_data )
  {
    if ( m_data = m_cache->m_fallback, m_data )
    {
      if ( m_state == ResourceState::Loading )
        m_state = ResourceState::LoadingFallback;
      else if ( m_state == ResourceState::NotFound )
        m_state = ResourceState::NotFoundFallback;
      else
        m_state = ResourceState::NotLoadedFallback;
    }
    else if ( m_state != ResourceState::Loading && m_state != ResourceState::NotFound )
      m_state = ResourceState::NotLoaded;
  }
}

// RTTR
template <typename Base, typename Specialised>
void Resource<Base, Specialised>::SetKey( const ResourceKey &key )
{
  m_key = key;
}

template <typename Base, typename Specialised>
const ResourceKey &Resource<Base, Specialised>::GetKey() const
{
  return m_key;
}

} // namespace Xivi