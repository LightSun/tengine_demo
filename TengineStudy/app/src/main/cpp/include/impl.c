//
// Created by Administrator on 2020/9/22 0022.
//
#include "tengine_c_api.h"

int prerun_graph_multithread2(graph_t graph, options ops){
    return prerun_graph_multithread(graph, ops.num_thread, ops.cluster);
}