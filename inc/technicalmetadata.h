#ifndef AFS_TECHNICAL_METADATA_H
#define AFS_TECHNICAL_METADATA_H

/*****************************************************************************
**
**  Definition of the Technical Metadata interface
**
**  Creation date:  2016/12/28
**  Created by:     Piql AS
**
**
**  Copyright (c) 2016 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include    "controlframe/boxingformat.h"
#include    "tocfiles.h"
#include    "boxing/bool.h"
#include    "mxml.h"

#ifdef __cplusplus
extern "C" {
#endif

// Definition structure afs_technical_metadata
typedef struct afs_technical_metadata_s
{
    afs_toc_files *     afs_tocs;
    afs_boxing_format * afs_content_boxing_format;
} afs_technical_metadata;

afs_technical_metadata * afs_technical_metadata_create();
afs_technical_metadata * afs_technical_metadata_create2(afs_toc_files * toc_files, afs_boxing_format * boxing_format);
void                     afs_technical_metadata_free(afs_technical_metadata*);

afs_technical_metadata * afs_technical_metadata_clone(const afs_technical_metadata * technical_metadata);
DBOOL                    afs_technical_metadata_equal(const afs_technical_metadata * technical_metadata1, const afs_technical_metadata * technical_metadata2);

DBOOL                    afs_technical_metadata_save_file(afs_technical_metadata * technical_metadata, const char * file_name, DBOOL compact);
char *                   afs_technical_metadata_save_string(afs_technical_metadata * technical_metadata, DBOOL compact);
DBOOL                    afs_technical_metadata_save_xml(afs_technical_metadata * technical_metadata, mxml_node_t * out);

DBOOL                    afs_technical_metadata_load_file(afs_technical_metadata * technical_metadata, const char * file_name);
DBOOL                    afs_technical_metadata_load_string(afs_technical_metadata * technical_metadata, const char * in);
DBOOL                    afs_technical_metadata_load_xml(afs_technical_metadata* technical_metadata, mxml_node_t * in);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TECHNICAL_METADATA_H
