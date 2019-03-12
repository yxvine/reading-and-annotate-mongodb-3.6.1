/*
 *    Copyright (C) 2012 10gen Inc.
 *
 *    This program is free software: you can redistribute it and/or  modify
 *    it under the terms of the GNU Affero General Public License, version 3,
 *    as published by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the GNU Affero General Public License in all respects
 *    for all of the code used other than as permitted herein. If you modify
 *    file(s) with this exception, you may extend this exception to your
 *    version of the file(s), but you are not obligated to do so. If you do not
 *    wish to do so, delete this exception statement from your version. If you
 *    delete this exception statement from all source files in the program,
 *    then also delete it in the license file.
 */

#pragma once

#include "mongo/base/init.h"
#include "mongo/util/fail_point_registry.h"

namespace mongo {

class FailPoint;

/**
 * @return the global fail point registry.
 */
FailPointRegistry* getGlobalFailPointRegistry();

/**
 * Convenience macro for declaring a fail point. Must be used in global scope and never in a
 * block with limited scope (ie, inside functions, loops, etc.).
 *
 * NOTE: Never use in header files, only sources.
 */
//例如MONGO_FP_DECLARE(shutdownAtStartup);
//定义一个GlobalInitializerRegisterer _mongoInitializerRegisterer_fp类，这些宏定义在main前执行
#define MONGO_FP_DECLARE(fp)                                                          \
    FailPoint fp;                                                                     \
    MONGO_INITIALIZER_GENERAL(fp, ("FailPointRegistry"), ("AllFailPointsRegistered")) \
    (::mongo::InitializerContext * context) {                                         \
        return getGlobalFailPointRegistry()->addFailPoint(#fp, &fp);                  \
    } //#fp，为fp字符串，fp为函数FailPoint类指针

/**
 * Convenience macro for defining a fail point in a header scope.
 */
#define MONGO_FP_FORWARD_DECLARE(fp) extern FailPoint fp;

/**
 * Convenience class for enabling a failpoint and disabling it as this goes out of scope.
 */
class FailPointEnableBlock {
public:
    FailPointEnableBlock(const std::string& failPointName);
    ~FailPointEnableBlock();

private:
    FailPoint* _failPoint;
};

}  // namespace mongo
