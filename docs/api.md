# API Reference (Public Headers)

- This document describes the public API surface exposed by AquaCrop-cpp.
- Public headers live under `include/` and are the contract for downstream users.

## Example
- Include a header: `#include <aquacrop/crop.hpp>`
- Link against the library produced by the build:

```
target_link_libraries(my_app PRIVATE aquacrop)
```

Notes:
- The API is designed to be stable and cross-platform; breaking changes require clear justification and a major version bump.
