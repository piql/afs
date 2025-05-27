#ifndef TOC_PREVIEWLAYOUTDEFINITIONS_H
#define TOC_PREVIEWLAYOUTDEFINITIONS_H

/*****************************************************************************
**
**  Definition of toc preview layout definitions C interface
**
**  Creation date:  2017/12/21
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include "previewlayoutdefinition.h" 

#ifdef __cplusplus
extern "C" {
#endif

typedef gvector afs_toc_preview_layout_definitions_vector;

typedef struct afs_toc_preview_layout_definitions_s
{
    afs_toc_preview_layout_definitions_vector *  layout_definitions;

    int reference_count;
} afs_toc_preview_layout_definitions;

afs_toc_preview_layout_definitions * afs_toc_preview_layout_definitions_create(void);
void                                 afs_toc_preview_layout_definitions_free(afs_toc_preview_layout_definitions * toc_preview_layout_definitions);

afs_toc_preview_layout_definitions * afs_toc_preview_layout_definitions_clone(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions);
afs_toc_preview_layout_definitions * afs_toc_preview_layout_definitions_get_new_reference(afs_toc_preview_layout_definitions * toc_preview_layout_definitions);
DBOOL                                afs_toc_preview_layout_definitions_equal(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions1, const afs_toc_preview_layout_definitions * toc_preview_layout_definitions2);
DBOOL                                afs_toc_preview_layout_definitions_is_valid(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions);

unsigned int                         afs_toc_preview_layout_definitions_get_count(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions);
afs_toc_preview_layout_definition *  afs_toc_preview_layout_definitions_get_layout_definition_by_index(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const unsigned int index);
afs_toc_preview_layout_definition *  afs_toc_preview_layout_definitions_get_layout_definition_by_id(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const char * id);
afs_toc_preview_layout_definition *  afs_toc_preview_layout_definitions_get_layout_definition_by_name(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const char * name);
DBOOL                                afs_toc_preview_layout_definitions_add_layout_definition(afs_toc_preview_layout_definitions * toc_preview_layout_definitions, afs_toc_preview_layout_definition * toc_preview_layout_definition);
DBOOL                                afs_toc_preview_layout_definitions_has_layout_definition(afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const char * id);

DBOOL                                afs_toc_preview_layout_definitions_save_file(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const char * file_name, DBOOL compact);
char *                               afs_toc_preview_layout_definitions_save_string(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions, DBOOL compact);
char *                               afs_toc_preview_layout_definitions_save_as_table(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions);
DBOOL                                afs_toc_preview_layout_definitions_save_xml(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions, struct mxml_node_s* out);

DBOOL                                afs_toc_preview_layout_definitions_load_file(afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const char * file_name);
DBOOL                                afs_toc_preview_layout_definitions_load_string(afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const char * in);
DBOOL                                afs_toc_preview_layout_definitions_load_xml(afs_toc_preview_layout_definitions * toc_preview_layout_definitions, struct mxml_node_s* input_node);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // TOC_PREVIEWLAYOUTDEFINITIONS_H
