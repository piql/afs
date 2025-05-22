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

#include "boxing/bool.h"
#include "boxing/platform/types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mxml_node_s;

void  afs_xmlutils_set_node_text(const char* text, struct mxml_node_s* node);
char* afs_xmlutils_get_node_text(struct mxml_node_s* node);
char* afs_xmlutils_get_node_text_safe( struct mxml_node_s* node, const char* default_text );
DBOOL afs_xmlutils_get_integer_attribute( int* int_storage, struct mxml_node_s* node, const char * name );
DBOOL afs_xmlutils_get_unsigned_integer_attribute(unsigned int* int_storage, struct mxml_node_s* node, const char * name);
void  afs_xmlutils_add_new_text_node(struct mxml_node_s* parent_node, const char * node_name, const char * text);
void  afs_xmlutils_add_new_int_node(struct mxml_node_s* parent_node, const char * node_name, int node_value);
void  afs_xmlutils_add_new_uint64_node(struct mxml_node_s* parent_node, const char * node_name, boxing_int64 node_value);
DBOOL afs_xmlutils_get_integer_element(int* int_storage, struct mxml_node_s* node, const char * name);
void afs_xmlutils_string_cut(char** string, size_t start_index, size_t end_index);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_XML_UTILS_H
