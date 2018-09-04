#ifndef AFS_XML_UTILS_H
#define AFS_XML_UTILS_H

/*****************************************************************************
**
**  Functions for working with XML.
**
**  Creation date:  2017/07/06
**  Created by:     Piql AS
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include "mxml.h"
#include "boxing/bool.h"
#include "boxing/platform/types.h"

#ifdef __cplusplus
extern "C" {
#endif

void  afs_xmlutils_set_node_text(const char* text, mxml_node_t* node);
char* afs_xmlutils_get_node_text(mxml_node_t* node);
DBOOL afs_xmlutils_get_integer_attribute(int* int_storage, mxml_node_t* node, const char * name);
DBOOL afs_xmlutils_get_unsigned_integer_attribute(unsigned int* int_storage, mxml_node_t* node, const char * name);
void  afs_xmlutils_add_new_text_node(mxml_node_t* parent_node, const char * node_name, const char * text);
void  afs_xmlutils_add_new_int_node(mxml_node_t* parent_node, const char * node_name, int node_value);
void  afs_xmlutils_add_new_uint64_node(mxml_node_t* parent_node, const char * node_name, boxing_int64 node_value);
DBOOL afs_xmlutils_get_integer_element(int* int_storage, mxml_node_t* node, const char * name);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_XML_UTILS_H
