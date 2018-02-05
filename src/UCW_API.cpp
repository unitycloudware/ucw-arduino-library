/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_API.h"

UCW_API::UCW_API(UCWConfig *config);
  _config = config;
}

UCW_API::~UCW_API() {
}
