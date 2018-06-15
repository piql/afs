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

#ifdef __cplusplus
extern "C" {
#endif

#include    "controlframe/boxingformat.h"
#include    "tocfiles.h"
#include    "boxing/bool.h"
#include    "mxml.h"

// Definition structure afs_technical_metadata
typedef struct afs_technical_metadata_s
{
    afs_toc_files * afs_tocs;
    afs_boxing_format * afs_content_boxing_format;
} afs_technical_metadata;

afs_technical_metadata* afs_technical_metadata_create();
void                    afs_technical_metadata_init(afs_technical_metadata*);
void                    afs_technical_metadata_init2(afs_technical_metadata*, afs_toc_files*, afs_boxing_format*);
void                    afs_technical_metadata_free(afs_technical_metadata*);

DBOOL                   afs_technical_metadata_save_xml(mxml_node_t * out, afs_technical_metadata* technical_metadata);
DBOOL                   afs_technical_metadata_load_xml(afs_technical_metadata* technical_metadata, mxml_node_t * in);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TECHNICAL_METADATA_H
