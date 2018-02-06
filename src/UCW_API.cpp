/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_API.h"

<<<<<<< HEAD
UCW_API::UCW_API(UCWConfig *config){
=======
UCW_API::UCW_API(UCWConfig *config);
>>>>>>> 152bfe44904665276f91064c2112ffb325094b57
  _config = config;
}

UCW_API::~UCW_API() {
}
<<<<<<< HEAD

String UCW_API::version() {
  sprintf(_version, "%d.%d.%d", UCW_VERSION_MAJOR, UCW_VERSION_MINOR, UCW_VERSION_PATCH);
  return _version;
}

String UCW_API::userAgent() {
  if (!_userAgent) {
    _userAgent = "UCW-Arduino/" + version();
  }

  return _userAgent;
}

String UCW_API::apiUrl() {

  return (_config->isSecuredConnection ? "https://" : "http://" ) + _config->host + UCW_API_PATH;

}

=======
>>>>>>> 152bfe44904665276f91064c2112ffb325094b57
