# libschauer

Qt based library to access the [Docker HTTP API](https://docs.docker.com/engine/api/latest/). It supports a subset of the Docker HTTP API - mainly API routes to create and start containers as well as execute commands inside running containers. Its current purpose is to manage containers used for running tests with [QTest](https://doc.qt.io/qt-5/qtest-overview.html) to test API libraries implementing access to services like Nextcloud. At the moment it only supports TCP sockets to talk to the docker daemon, unix sockets are not supported.

Read more in the [API documentation for libschauer](https://doc.huessenbergnetz.de/libschauer/?pk_campaign=Github-Project-Libschauer&pk_kwd=ReadmeFile).

## Features
* list installed images
* create, start, stop and remove containers
* execute commands inside containers

## Contributing
The source code is available on [Github](https://github.com/Huessenbergnetz/libschauer/), feel free to clone or branch according to the [LGPLv3](https://github.com/Huessenbergnetz/libschauer/blob/master/COPYING.LESSER). Translation is done on [Transifex](https://www.transifex.com/huessenbergnetz/libschauer).

## Getting libschauer
* [openSUSE](https://software.opensuse.org/package/libSchauerQt5-0)

## Building libschauer
Either clone the git repository or download one of the [release](https://github.com/Huessenbergnetz/libschauer/releases) tarballs. libschauer is based on the KJob implementation of KDE’s KCoreAddons Qt library. For systems where this is not easily available, a custom implmementation is included.

### Build requirements
* [CMake](https://cmake.org/)
* [Qt5Core](https://www.qt.io/)
* Qt5Network
* Qt5LinguistTools

### Optional required but recommended
* [KF5CoreAddons](https://api.kde.org/frameworks/kcoreaddons/html/index.html)
* Qt5Test
* doxygen and dot

### CMake switches

| Option                  | Default Value | Requirements                  | Description
|-------------------------|---------------|-------------------------------|-------------
| BUILD_DOCS              | OFF           | doxygen and dot               | Build API documentation
| BUILD_DOCS_QUIET        | OFF           |                               | Be quiet when building API docs
| ENABLE_MAINTAINER_FLAGS | OFF           |                               | Enable some compiler flags for more warnings
| WITH_KDE                | ON            | KF5CoreAddons                 | Use KF5CoreAddeons KJob implementation
| WITH_TESTS              | OFF           | QTest                         | Build unit tests
| WITH_API_TESTS          | OFF           | docker listening on TCP port  | Build API tests, currently require nginx image installed

### Additional make targes
When `BUILD_DOCS` is enabled, additional build targets are available.

#### webdocs
Will create HTML documentation files linked to Qt online documentation if correct tag files are avaiable.

#### htmldocs
Will create HTML documentation files linked to local Qt offline HTML documentation if correct tag files are available.

#### qtdocs
Will create compiled Qt documentation usable inside Qt creator if qhelpgenerator is available.

## License

```
libschauer - Qt based library to access the Docker HTTP API
Copyright (C) 2021-2022 Hüssenbergnetz/Matthias Fehring
https://github.com/Huessenbergnetz/libschauer

This library is free software: you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
```
