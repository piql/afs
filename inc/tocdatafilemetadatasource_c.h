#ifndef AFS_TOC_DATA_FILE_METADATA_SOURCE_H
#define AFS_TOC_DATA_FILE_METADATA_SOURCE_H

/*****************************************************************************
**
**  Definition of the toc data file metadata source interface
**
**  Creation date:  2018/01/23
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

#include "boxing/config.h"
#include "gvector.h"
#include "mxml.h"

typedef gvector afs_toc_data_file_metadata_sources;

// Definition structure afs_toc_data_file_metadata_source
typedef struct afs_toc_data_file_metadata_source_s
{
    char * data;
    char * format_id;
    int    file_id;
    int    source_id;
} afs_toc_data_file_metadata_source;

afs_toc_data_file_metadata_source * afs_toc_data_file_metadata_source_create();
afs_toc_data_file_metadata_source * afs_toc_data_file_metadata_source_create2(const char * data, const char * format_id, const int file_id, const int source_id);
void                                afs_toc_data_file_metadata_source_init(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * data, const char * format_id, const int file_id, const int source_id);
void                                afs_toc_data_file_metadata_source_free(afs_toc_data_file_metadata_source * toc_data_file_metadata_source);

afs_toc_data_file_metadata_source * afs_toc_data_file_metadata_source_clone(afs_toc_data_file_metadata_source * toc_data_file_metadata_source);
DBOOL                               afs_toc_data_file_metadata_source_equal(afs_toc_data_file_metadata_source * toc_data_file_metadata_source1, afs_toc_data_file_metadata_source * toc_data_file_metadata_source2);
DBOOL                               afs_toc_data_file_metadata_source_is_valid(afs_toc_data_file_metadata_source * toc_data_file_metadata_source);

void                                afs_toc_data_file_metadata_source_set_data(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * data);
void                                afs_toc_data_file_metadata_source_set_format_id(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * format_id);

DBOOL                               afs_toc_data_file_metadata_source_save_file(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * file_name, DBOOL compact);
char *                              afs_toc_data_file_metadata_source_save_string(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, DBOOL compact);
DBOOL                               afs_toc_data_file_metadata_source_save_xml(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, mxml_node_t* out);

DBOOL                               afs_toc_data_file_metadata_source_load_file(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * file_name);
DBOOL                               afs_toc_data_file_metadata_source_load_string(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * in);
DBOOL                               afs_toc_data_file_metadata_source_load_xml(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, mxml_node_t* input_node);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TOC_DATA_FILE_METADATA_SOURCE_H