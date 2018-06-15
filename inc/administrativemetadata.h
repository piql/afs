#ifndef AFS_ADMINISTRATIVE_METADATA_H
#define AFS_ADMINISTRATIVE_METADATA_H

/*****************************************************************************
**
**  Definition of the Administrative Metadata interface
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

#include    "boxing/bool.h"
#include    "mxml.h"

// Definition structure afs_administrative_metadata
typedef struct afs_administrative_metadata_s
{
    char * reel_id;
    char * print_reel_id;
    char * title;
    char * description;
    char * creator;
    char * creation_date;
} afs_administrative_metadata;


afs_administrative_metadata* afs_administrative_metadata_create();
void                         afs_administrative_metadata_init(afs_administrative_metadata*);
void                         afs_administrative_metadata_init2(
                                 afs_administrative_metadata* administrative_metadata,
                                 const char* reel_id,
                                 const char* print_reel_id,
                                 const char* title,
                                 const char* description,
                                 const char* creator,
                                 const char* creation_date);
void                         afs_administrative_metadata_free(afs_administrative_metadata*);

DBOOL                        afs_administrative_metadata_save_xml(mxml_node_t * out, afs_administrative_metadata* control_data);
DBOOL                        afs_administrative_metadata_load_xml(afs_administrative_metadata* control_data, mxml_node_t * in);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_ADMINISTRATIVE_METADATA_H
