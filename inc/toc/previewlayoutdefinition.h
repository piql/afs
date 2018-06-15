#ifndef TOC_PREVIEWLAYOUTDEFINITION_H
#define TOC_PREVIEWLAYOUTDEFINITION_H

/*****************************************************************************
**
**  Definition of toc preview layout definition C interface
**
**  Creation date:  2016/08/18
**  Created by:     Ole Liabo
**
**
**  Copyright (c) 2016 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include "toc/previewsections.h" 

#ifdef __cplusplus
extern "C" {
#endif

typedef struct afs_toc_preview_layout_definition_s
{
    char *                      id;
    char *                      name;
    afs_toc_preview_sections *  sections;
} afs_toc_preview_layout_definition;

afs_toc_preview_layout_definition * afs_toc_preview_layout_definition_create();
afs_toc_preview_layout_definition * afs_toc_preview_layout_definition_create2(const char * id, const char * name);
afs_toc_preview_layout_definition * afs_toc_preview_layout_definition_create3(const char * id, const char * name, afs_toc_preview_sections * sections);
afs_toc_preview_layout_definition * afs_toc_preview_layout_definition_create4(mxml_node_t* definition_node);
void                                afs_toc_preview_layout_definition_init(afs_toc_preview_layout_definition * toc_preview_layout_definition, const char * id, const char * name);
void                                afs_toc_preview_layout_definition_init2(afs_toc_preview_layout_definition * toc_preview_layout_definition, const char * id, const char * name, afs_toc_preview_sections * sections);
void                                afs_toc_preview_layout_definition_free(afs_toc_preview_layout_definition * toc_preview_layout_definition);

afs_toc_preview_layout_definition * afs_toc_preview_layout_definition_clone(const afs_toc_preview_layout_definition * toc_preview_layout_definition);
DBOOL                               afs_toc_preview_layout_definition_equal(const afs_toc_preview_layout_definition * toc_preview_layout_definition1, const afs_toc_preview_layout_definition * toc_preview_layout_definition2);
DBOOL                               afs_toc_preview_layout_definition_is_valid(const afs_toc_preview_layout_definition * toc_preview_layout_definition);

unsigned int                        afs_toc_preview_layout_definition_get_section_count(const afs_toc_preview_layout_definition * toc_preview_layout_definition);
afs_toc_preview_section *           afs_toc_preview_layout_definition_get_section(const afs_toc_preview_layout_definition * toc_preview_layout_definition, unsigned int index);
DBOOL                               afs_toc_preview_layout_definition_add_section(afs_toc_preview_layout_definition * toc_preview_layout_definition, afs_toc_preview_section * toc_preview_section);
DBOOL                               afs_toc_preview_layout_definition_is_section_size_equal(const afs_toc_preview_layout_definition * toc_preview_layout_definition);

DBOOL                               afs_toc_preview_layout_definition_save_file(const afs_toc_preview_layout_definition * toc_preview_layout_definition, const char * file_name, DBOOL compact);
char *                              afs_toc_preview_layout_definition_save_string(const afs_toc_preview_layout_definition * toc_preview_layout_definition, DBOOL compact);
DBOOL                               afs_toc_preview_layout_definition_save_xml(const afs_toc_preview_layout_definition * toc_preview_layout_definition, mxml_node_t* out);

DBOOL                               afs_toc_preview_layout_definition_load_file(afs_toc_preview_layout_definition * toc_preview_layout_definition, const char * file_name);
DBOOL                               afs_toc_preview_layout_definition_load_string(afs_toc_preview_layout_definition * toc_preview_layout_definition, const char * in);
DBOOL                               afs_toc_preview_layout_definition_load_xml(afs_toc_preview_layout_definition * toc_preview_layout_definition, mxml_node_t* input_node);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // TOC_PREVIEWLAYOUTDEFINITION_H
