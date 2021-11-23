#pragma once

namespace Xivi
{
  enum class ResourceState : u8
  {
    NotLoaded,
    NotLoadedFallback,
    Loading,
    LoadingFallback,
    NotFound,
    NotFoundFallback,
    Mutable,
    Final
  };

  enum class ResourceLifetime : u8
  {
    Persistant,
    Referenced,
    Managed
  };
}