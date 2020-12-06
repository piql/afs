#ifndef AFS_TOC_METADATA_H
#define AFS_TOC_METADATA_H

/*****************************************************************************
**
**  Definition of the toc metadata interface
**
**  Creation date:  2017/09/26
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include "tocmetadatasource_c.h"
#include "boxing/config.h"
#include "gvector.h"

#ifdef __cplusplus
extern "C" {
#endif

// Definition structure afs_toc_file
typedef struct afs_toc_metadata_s
{
    afs_toc_metadata_sources * sources;
} afs_toc_metadata;

afs_toc_metadata * afs_toc_metadata_create();
afs_toc_metadata * afs_toc_metadata_create2(afs_toc_metadata_sources * sources);
afs_toc_metadata * afs_toc_metadata_create3(struct mxml_node_s* sources_node);

void               afs_toc_metadata_init(afs_toc_metadata * toc_metadata);
void               afs_toc_metadata_init2(afs_toc_metadata * toc_metadata, afs_toc_metadata_sources * sources);

void               afs_toc_metadata_free(afs_toc_metadata * toc_metadata);

afs_toc_metadata * afs_toc_metadata_clone(afs_toc_metadata * toc_metadata);
DBOOL              afs_toc_metadata_equal(afs_toc_metadata * toc_metadata1, afs_toc_metadata * toc_metadata2);

DBOOL                     afs_toc_metadata_add_source(afs_toc_metadata * toc_metadata, afs_toc_metadata_source * toc_metadata_source);
afs_toc_metadata_source * afs_toc_metadata_get_source(afs_toc_metadata * toc_metadata, unsigned int index);
unsigned int              afs_toc_metadata_get_sources_count(afs_toc_metadata * toc_metadata);
DBOOL                     afs_toc_metadata_is_valid(afs_toc_metadata * toc_metadata);

DBOOL              afs_toc_metadata_save_file(afs_toc_metadata * toc_metadata, const char * file_name, DBOOL compact);
char *             afs_toc_metadata_save_string(afs_toc_metadata * toc_metadata, DBOOL compact);
DBOOL              afs_toc_metadata_save_xml(afs_toc_metadata * toc_metadata, struct mxml_node_s* out);

DBOOL              afs_toc_metadata_load_file(afs_toc_metadata * toc_metadata, const char * file_name);
DBOOL              afs_toc_metadata_load_string(afs_toc_metadata * toc_metadata, const char * in);
DBOOL              afs_toc_metadata_load_xml(afs_toc_metadata * toc_metadata, struct mxml_node_s* node);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TOC_METADATA_H
