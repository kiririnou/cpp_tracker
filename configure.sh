#!/bin/sh
set -e
mkdir bin/
cp libs/* bin/
git clone https://github.com/Tencent/rapidjson.git vendor/rapidjson
git clone https://github.com/gabime/spdlog.git vendor/spdlog
