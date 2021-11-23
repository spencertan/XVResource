#pragma once

#include <string_view>
#include <Utils/XXH3.h>
namespace Xivi
{
class ResourceKey
{
public:
  // Possible to go lightweight and have an external meta mapping to hold string and the digest
  using Digest = u64;
  using StringKey = std::string;

  ResourceKey() = default;

  explicit ResourceKey( const Digest digest ) :
    m_digest( digest )
  {}

  constexpr ResourceKey( const StringKey &str ) :
    m_digest( Hash::XXH3_64( str ) ), m_key { str }
  {
  }

  template <u64 Size>
  constexpr ResourceKey( const char( &str )[Size] ) :
    m_digest( Hash::XXH3_64( str ) ), m_key { str }
  {
  }

  bool operator==( const ResourceKey &rhs ) const
  {
    return m_digest == rhs.m_digest;
  }

  bool operator!=( const ResourceKey &rhs ) const
  {
    return m_digest != rhs.m_digest;
  }

  size_t operator()() const
  {
    return m_digest;
  }

  const StringKey &Key() const
  {
    return m_key;
  }

  const StringKey &GetKey() const
  {
    return m_key;
  }

  void SetKey( const StringKey &str )
  {
    m_key = str;
  }

  const Digest &GetDigest() const
  {
    return m_digest;
  }

  void SetDigest( const Digest &digest )
  {
    m_digest = digest;
  }

private:
  Digest m_digest { 0 };
  StringKey m_key {};
};
} // namespace Xivi

namespace std
{
template <>
struct hash<Xivi::ResourceKey>
{
  std::size_t operator()( const Xivi::ResourceKey &key ) const
  {
    return key();
  }
};
} // namespace std