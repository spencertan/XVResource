namespace Xivi
{
  template <typename Base>
  IResourceLoader<Base>::~IResourceLoader()
  {
    if (m_cache)
      m_cache->m_loader = nullptr;
  }

  template <typename Base>
  u64 IResourceLoader<Base>::RequestCount() const
  {
    return m_requests;
  }

  template <typename Base>
  u64 IResourceLoader<Base>::LoadedCount() const
  {
    return m_loaded;
  }

  template <typename Base>
  u64 IResourceLoader<Base>::NotFoundCount() const
  {
    return m_notFound;
  }

  template <typename Base>
  void IResourceLoader<Base>::Load(ResourceKey key)
  {
    m_requests++;
    DoLoad(key);
  }

  template <typename Base>
  void IResourceLoader<Base>::Load( ResourceKey key, std::string_view str )
  {
    m_requests++;
    DoLoad(key, str);
  }

  template <typename Base>
  void IResourceLoader<Base>::Set(ResourceKey key, Ptr<Base> data, ResourceState state, ResourceLifetime lifetime)
  {
    if (data)
      m_loaded++;

    if (!data && state == ResourceState::NotFound)
      m_notFound++;

    m_cache->Set(key, data, state, lifetime);
  }

  template <typename Base>
  template <typename Specialised>
  requires(!std::is_null_pointer_v<Raw<Specialised>>)
  void IResourceLoader<Base>::Set(ResourceKey key, Specialised &&data, ResourceState state, ResourceLifetime lifetime)
  {
    Set(key, new Raw<Specialised>(std::forward<Specialised>(data)), state, lifetime);
  }

  template <typename Base>
  void IResourceLoader<Base>::Set(ResourceKey key, Ptr<Base> data)
  {
    Set(key, data, ResourceState::Final, ResourceLifetime::Persistant);
  }

  template <typename Base>
  template <typename Specialised>
  requires(!std::is_null_pointer_v<Raw<Specialised>>)
  void IResourceLoader<Base>::Set(ResourceKey key, Specialised &&data)
  {
    Set(key, new Raw<Specialised>(std::forward<Specialised>(data)));
  }

  template <typename Base>
  void IResourceLoader<Base>::SetNotFound(ResourceKey key)
  {
    Set(key, nullptr, ResourceState::NotFound, ResourceLifetime::Persistant);
  }

} // namespace Xivi