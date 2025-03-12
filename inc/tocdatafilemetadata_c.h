#ifndef AFS_TOC_DATA_FILE_METADATA_H
#define AFS_TOC_DATA_FILE_METADATA_H

/*****************************************************************************
**
**  Definition of the toc data file metadata interface
**
**  Creation date:  2017/01/02
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include "tocdatafilemetadatasource_c.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct afs_toc_data_file_metadata_s
{
    afs_toc_data_file_metadata_sources * sources;

    int reference_count;
} afs_toc_data_file_metadata;

afs_toc_data_file_metadata * afs_toc_data_file_metadata_create();
afs_toc_data_file_metadata * afs_toc_data_file_metadata_create2(afs_toc_data_file_metadata_sources * sources);

void                         afs_toc_data_file_metadata_init(afs_toc_data_file_metadata * toc_data_file_metadata);
void                         afs_toc_data_file_metadata_init2(afs_toc_data_file_metadata * toc_data_file_metadata, afs_toc_data_file_metadata_sources * sources);

void                         afs_toc_data_file_metadata_free(afs_toc_data_file_metadata * toc_data_file_metadata);

afs_toc_data_file_metadata * afs_toc_data_file_metadata_clone(const afs_toc_data_file_metadata * toc_data_file_metadata);
afs_toc_data_file_metadata * afs_toc_data_file_metadata_get_new_reference(afs_toc_data_file_metadata * toc_data_file_metadata);
DBOOL                        afs_toc_data_file_metadata_equal(const afs_toc_data_file_metadata * toc_data_file_metadata1, const afs_toc_data_file_metadata * toc_data_file_metadata2);

DBOOL                               afs_toc_data_file_metadata_add_source(afs_toc_data_file_metadata * toc_data_file_metadata, afs_toc_data_file_metadata_source * toc_data_file_metadata_source);
afs_toc_data_file_metadata_source * afs_toc_data_file_metadata_get_source(const afs_toc_data_file_metadata * toc_data_file_metadata, unsigned int index);

unsigned int                 afs_toc_data_file_metadata_get_sources_count(const afs_toc_data_file_metadata * toc_data_file_metadata);
DBOOL                        afs_toc_data_file_metadata_is_valid(const afs_toc_data_file_metadata * toc_data_file_metadata);

DBOOL                        afs_toc_data_file_metadata_save_file(afs_toc_data_file_metadata * toc_data_file_metadata, const char * file_name, DBOOL compact);
char *                       afs_toc_data_file_metadata_save_string(afs_toc_data_file_metadata * toc_data_file_metadata, DBOOL compact);
char *                       afs_toc_data_file_metadata_save_as_table(const afs_toc_data_file_metadata * toc_data_file_metadata);
DBOOL                        afs_toc_data_file_metadata_save_xml(afs_toc_data_file_metadata * toc_data_file_metadata, mxml_node_t* out);

DBOOL                        afs_toc_data_file_metadata_load_file(afs_toc_data_file_metadata * toc_data_file_metadata, const char * file_name);
DBOOL                        afs_toc_data_file_metadata_load_string(afs_toc_data_file_metadata * toc_data_file_metadata, const char * in);
DBOOL                        afs_toc_data_file_metadata_load_xml(afs_toc_data_file_metadata * toc_data_file_metadata, mxml_node_t* node);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TOC_DATA_FILE_METADATA_H
