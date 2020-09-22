//
// Created by Administrator on 2020/9/22 0022.
//
#include <malloc.h>
#include "string.h"
#include "tengine_c_api.h"

int prerun_graph_multithread2(graph_t graph, options ops){
    return prerun_graph_multithread(graph, ops.num_thread, ops.cluster);
}

const char* concatStr(const char *s1, const char *s2){
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    if (result == NULL)
        return NULL;
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}