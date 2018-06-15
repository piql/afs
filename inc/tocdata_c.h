#ifndef AFS_TOC_DATA_H
#define AFS_TOC_DATA_H

/*****************************************************************************
**
**  Definition of the TOC data interface
**
**  Creation date:  2018/05/10
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2018 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "tocdatareels.h"
#include "tocmetadata_c.h"
#include "mxml.h"

typedef struct afs_toc_data_s
{
    char *                               version;
    char *                               index_type;
    char *                               job_id;
    char *                               reel_id;
    afs_toc_data_reels *                 reels;
    afs_toc_metadata *                   metadata;
    afs_toc_preview_layout_definitions * preview_layout_definitions;
} afs_toc_data;

afs_toc_data * afs_toc_data_create();
afs_toc_data * afs_toc_data_create2(const char * version, const char * index_type, const char * job_id, const char * reel_id);
void           afs_toc_data_free(afs_toc_data * toc_data);

afs_toc_data * afs_toc_data_clone(afs_toc_data * toc_data);
DBOOL          afs_toc_data_equal(afs_toc_data * toc_data1, afs_toc_data * toc_data2);

afs_toc_data_reel  *                afs_toc_data_get_reel_by_index(const afs_toc_data * toc_data, const unsigned int index);
afs_toc_data_reel  *                afs_toc_data_get_reel_by_id(const afs_toc_data * toc_data, const char * id);
afs_toc_metadata_source  *          afs_toc_data_get_metadata_source(const afs_toc_data * toc_data, const unsigned int index);
afs_toc_file  *                     afs_toc_data_get_file_by_id(const afs_toc_data * toc_data, const int id);
afs_toc_file  *                     afs_toc_data_get_file_by_unique_id(const afs_toc_data * toc_data, const char * unique_id);
afs_toc_preview_layout_definition * afs_toc_data_get_preview_layout_definition(const afs_toc_data * toc_data, const unsigned int index);

void afs_toc_data_set_version(afs_toc_data * toc_data, const char * version);
void afs_toc_data_set_index_type(afs_toc_data * toc_data, const char * index_type);
void afs_toc_data_set_job_id(afs_toc_data * toc_data, const char * job_id);
void afs_toc_data_set_reel_id(afs_toc_data * toc_data, const char * reel_id);

DBOOL afs_toc_data_add_reel(afs_toc_data * toc_data, afs_toc_data_reel * toc_data_reel);
DBOOL afs_toc_data_add_metadata_source(afs_toc_data * toc_data, afs_toc_metadata_source * toc_metadata_source);
DBOOL afs_toc_data_add_preview_layout_definition(afs_toc_data * toc_data, afs_toc_preview_layout_definition * toc_preview_layout_definition);
DBOOL afs_toc_data_add_preview_layout_definitions(afs_toc_data * toc_data, const afs_toc_preview_layout_definitions * toc_preview_layout_definitions);

unsigned int afs_toc_data_reel_count(const afs_toc_data * toc_data);
unsigned int afs_toc_data_metadata_source_count(const afs_toc_data * toc_data);
unsigned int afs_toc_data_preview_layout_definition_count(const afs_toc_data * toc_data);
unsigned int afs_toc_data_file_count(const afs_toc_data * toc_data);
unsigned int afs_toc_data_frame_count(const afs_toc_data * toc_data);
int          afs_toc_data_first_frame(const afs_toc_data * toc_data);
int          afs_toc_data_last_frame(const afs_toc_data * toc_data);
DBOOL        afs_toc_data_is_valid(const afs_toc_data * toc_data);
void         afs_toc_data_add_frame_offset(afs_toc_data * toc_data, unsigned int offset, const unsigned int section);

DBOOL  afs_toc_data_save_file(afs_toc_data * toc_data, const char * file_name, DBOOL compact);
char * afs_toc_data_save_string(afs_toc_data * toc_data, DBOOL compact);
DBOOL  afs_toc_data_save_xml(afs_toc_data * toc_data, mxml_node_t* out);

DBOOL  afs_toc_data_load_file(afs_toc_data * toc_data, const char * file_name);
DBOOL  afs_toc_data_load_string(afs_toc_data * toc_data, const char * in);
DBOOL  afs_toc_data_load_xml(afs_toc_data * toc_data, mxml_node_t* node);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TOC_DATA_H
