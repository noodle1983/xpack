﻿/*
* Copyright (C) 2021 Duowan Inc. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef __X_PACK_JSON_ENCODER_H
#define __X_PACK_JSON_ENCODER_H

#include <string>

#include "rapidjson_custom.h"
#include "thirdparty/rapidjson/prettywriter.h"
#include "thirdparty/rapidjson/stringbuffer.h"

#include "xencoder.h"

namespace xpack {

class JsonEncoder:public XEncoder<JsonEncoder>, private noncopyable {
    typedef rapidjson::StringBuffer JSON_WRITER_BUFFER;
    typedef rapidjson::Writer<rapidjson::StringBuffer> JSON_WRITER_WRITER;
    typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> JSON_WRITER_PRETTY;
public:
    friend class XEncoder<JsonEncoder>;
    using xdoc_type::encode;

    JsonEncoder(int indentCount=-1, char indentChar=' ') {
        _buf = new JSON_WRITER_BUFFER;
        if (indentCount < 0) {
            _writer = new JSON_WRITER_WRITER(*_buf);
            _pretty = NULL;
        } else {
            _pretty = new JSON_WRITER_PRETTY(*_buf);
            _pretty->SetIndent(indentChar, indentCount);
            _writer = NULL;
        }
    }
    ~JsonEncoder() {
        if (NULL != _buf) {
            delete _buf;
            _buf = NULL;
        }
        if (NULL != _writer) {
            delete _writer;
            _writer = NULL;
        }
        if (NULL != _pretty) {
            delete _pretty;
            _pretty = NULL;
        }
    }

    inline const char *Type() const {
        return "json";
    }
    inline const char *IndexKey(size_t index) {
        (void)index;
        return NULL;
    }

    std::string String() {
        return _buf->GetString();
    }

public:
    void ArrayBegin(const char *key, const Extend *ext) {
        (void)ext;
        xpack_set_key(key);
        if (NULL != _writer) {
            _writer->StartArray();
        } else {
            _pretty->StartArray();
        }
    }
    void ArrayEnd(const char *key, const Extend *ext) {
        (void)key;
        (void)ext;
        if (NULL != _writer) {
            _writer->EndArray();
        } else {
            _pretty->EndArray();
        }
    }
    void ObjectBegin(const char *key, const Extend *ext) {
        (void)ext;
        xpack_set_key(key);
        if (NULL != _writer) {
            _writer->StartObject();
        } else {
            _pretty->StartObject();
        }
    }
    void ObjectEnd(const char *key, const Extend *ext) {
        (void)key;
        (void)ext;
        if (NULL != _writer) {
            _writer->EndObject();
        } else {
            _pretty->EndObject();
        }
    }

public:
    #define X_PACK_JSON_ENCODE(cond, f)  \
        if ((cond) && Extend::OmitEmpty(ext)){ \
            return false;                \
        }                                \
        xpack_set_key(key);              \
        if (NULL != _writer) {           \
            _writer->f(val);             \
        } else {                         \
            _pretty->f(val);             \
        }                                \
        return true

    bool writeNull(const char*key, const Extend *ext) {
        if (Extend::OmitEmpty(ext)) {
            return false;
        }
        xpack_set_key(key);
        if (NULL != _writer) {
            _writer->Null();
        } else {
            _pretty->Null();
        }
        return true;
    }
    template<typename T>
    bool empty(const T &) { return false; }
    bool empty(const std::string &val) { return val.empty(); }
    bool encode(const char*key, const std::string &val, const Extend *ext) {
        X_PACK_JSON_ENCODE(val.empty(), String);
    }
    bool empty(const bool &val) { return !val; }
    bool encode(const char*key, const bool &val, const Extend *ext) {
        X_PACK_JSON_ENCODE(!val, Bool);
    }
    bool encode(const char*key, const char &val, const Extend *ext) {
        return this->encode(key, (const int&)val, ext);
    }
    bool encode(const char*key, const signed char &val, const Extend *ext) {
        return this->encode(key, (const int&)val, ext);
    }
    bool encode(const char*key, const unsigned char &val, const Extend *ext) {
        return this->encode(key, (const unsigned int&)val, ext);
    }
    bool encode(const char*key, const short & val, const Extend *ext) {
        return this->encode(key, (const int&)val, ext);
    }
    bool encode(const char*key, const unsigned short & val, const Extend *ext) {
        return this->encode(key, (const unsigned int&)val, ext);
    }
    bool empty(const int &val) { return val==0; }
    bool encode(const char*key, const int& val, const Extend *ext) {
        X_PACK_JSON_ENCODE(val==0, Int);
    }
    bool empty(const unsigned int &val) { return val==0; }
    bool encode(const char*key, const unsigned int& val, const Extend *ext) {
        X_PACK_JSON_ENCODE(val==0, Uint);
    }
    bool empty(const long long &val) { return val==0; }
    bool encode(const char*key, const long long& val, const Extend *ext) {
        X_PACK_JSON_ENCODE(val==0, Int64);
    }
    bool empty(const unsigned long long &val) { return val==0; }
    bool encode(const char*key, const unsigned long long & val, const Extend *ext) {
        X_PACK_JSON_ENCODE(val==0, Uint64);
    }
    bool empty(const long &val) { return val==0; }
    bool encode(const char*key, const long &val, const Extend *ext) {
        return this->encode(key, (const long long&)val, ext);
    }
    bool empty(const unsigned long &val) { return val==0; }
    bool encode(const char*key, const unsigned long &val, const Extend *ext) {
        return this->encode(key, (const unsigned long long&)val, ext);
    }
    bool empty(const float &val) { return val==0; }
    bool encode(const char*key, const float & val, const Extend *ext) {
        X_PACK_JSON_ENCODE(val==0, Double);
    }
    bool empty(const double &val) { return val==0; }
    bool encode(const char*key, const double & val, const Extend *ext) {
        X_PACK_JSON_ENCODE(val==0, Double);
    }
    bool empty(const long double &val) { return val==0; }
    bool encode(const char*key, const long double & val, const Extend *ext) {
        X_PACK_JSON_ENCODE(val==0, Double);
    }

    // map<int, T> xml not support use number as label
    template <class K, class T>
    typename x_enable_if<numeric<K>::is_integer, bool>::type encode(const char*key, const std::map<K,T>& val, const Extend *ext) {
        std::map<std::string,T> tmpv;

        for (typename std::map<K,T>::const_iterator iter = val.begin(); iter!=val.end(); ++iter) {
            tmpv[Util::itoa(iter->first)] = iter->second;
        }
        return this->encode(key, tmpv, ext);
    }
    #ifdef XPACK_SUPPORT_QT
    template <class K, class T>
    typename x_enable_if<numeric<K>::is_integer, bool>::type encode(const char*key, const QMap<K,T>& val, const Extend *ext) {
        std::map<std::string,T> tmpv;

        for (typename QMap<K,T>::const_iterator iter = val.constBegin(); iter!=val.constEnd(); ++iter) {
            tmpv[Util::itoa(iter.key())] = iter.value();
        }
        return this->encode(key, tmpv, ext);
    }
    #endif
private:
    void xpack_set_key(const char*key) { // openssl defined set_key macro, so we named it xpack_set_key
        if (NULL!=key && key[0]!='\0') {
            if (NULL != _writer) {
                _writer->Key(key);
            } else {
                _pretty->Key(key);
            }
        }
    }

    JSON_WRITER_BUFFER* _buf;
    JSON_WRITER_WRITER* _writer;
    JSON_WRITER_PRETTY* _pretty;
};

}

#endif
