/*
 *  (c) Copyright 2016-2017 Hewlett Packard Enterprise Development Company LP.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  As an exception, the copyright holders of this Library grant you permission
 *  to (i) compile an Application with the Library, and (ii) distribute the
 *  Application containing code generated by the Library and added to the
 *  Application during this compilation process under terms of your choice,
 *  provided you also meet the terms and conditions of the Application license.
 *
 */

#include "kvs_client/kvs_client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <random>
#include <cstddef> // size_t

using namespace radixtree;

std::random_device r;
std::default_random_engine e1(r());
uint64_t rand_uint64(uint64_t min, uint64_t max)
{
    std::uniform_int_distribution<uint64_t> uniform_dist(min, max);
    return uniform_dist(e1);
}

std::string rand_string(size_t min_len, size_t max_len)
{
    static char const dict[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    size_t len = (size_t)rand_uint64(min_len, max_len);
    std::string ret(len, '\0');
    for (size_t i = 0; i < len; i++)
    {
        ret[i]=dict[rand_uint64(0,sizeof(dict)-2)];
    }

    return ret;
}

int main (int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: client path\n");
        exit(1);
    }

    std::string path = argv[1];

    KVSServer s;
    int ret = s.Init(path);
    assert(ret==0);

    s.PrintCluster();

    size_t max_key_len = 8;
    size_t max_val_len = 32;

    for(int i=0; i<10; i++) {
        std::string key = rand_string(1, max_key_len);
        std::string val = rand_string(1, max_val_len);

        ret = s.Put(key.c_str(), key.size(), val.c_str(), val.size());
        assert(ret==0);

        //char key_buf[max_key_len];
        char val_buf[max_val_len];
        size_t val_len;
        ret = s.Get(key.c_str(), key.size(), val_buf, val_len);
        assert(ret==0);
    }

    s.Final();

    return 0;
}