#ifndef AFS_TOC_DATA_REELS_H
#define AFS_TOC_DATA_REELS_H

/*****************************************************************************
**
**  Definition of the TOC data reels interface
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

#include "tocdatareel_c.h"
#include "toc/previewlayoutdefinitions.h"
#include "gvector.h"

#ifdef __cplusplus
extern "C" {
#endif

// Definition structure afs_toc_files
typedef gvector afs_toc_data_reels_vector;

typedef struct afs_toc_data_reels_s
{
    afs_toc_data_reels_vector * reels;

    int reference_count;
} afs_toc_data_reels;

afs_toc_data_reels * afs_toc_data_reels_create();
afs_toc_data_reels * afs_toc_data_reels_create2(afs_toc_data_reels_vector * reels);

void                 afs_toc_data_reels_free(afs_toc_data_reels * toc_data_reels);

afs_toc_data_reels * afs_toc_data_reels_clone(afs_toc_data_reels * toc_data_reels);
afs_toc_data_reels * afs_toc_data_reels_get_new_reference(afs_toc_data_reels * toc_data_reels);
DBOOL                afs_toc_data_reels_equal(afs_toc_data_reels * toc_data_reels1, afs_toc_data_reels * toc_data_reels2);

DBOOL                afs_toc_data_reels_add_reel(afs_toc_data_reels * toc_data_reels, afs_toc_data_reel * toc_data_reel);
afs_toc_data_reel  * afs_toc_data_reels_get_reel(const afs_toc_data_reels * toc_data_reels, const unsigned int index);
unsigned int         afs_toc_data_reels_get_reels_count(const afs_toc_data_reels * toc_data_reels);
DBOOL                afs_toc_data_reels_is_valid(afs_toc_data_reels * toc_data_reels);

DBOOL                afs_toc_data_reels_save_file(afs_toc_data_reels * toc_data_reels, const char * file_name, DBOOL compact);
char *               afs_toc_data_reels_save_string(afs_toc_data_reels * toc_data_reels, DBOOL compact);
DBOOL                afs_toc_data_reels_save_xml(afs_toc_data_reels * toc_data_reels, struct mxml_node_s* out);

DBOOL                afs_toc_data_reels_load_file(afs_toc_data_reels * toc_data_reels, const char * file_name);
DBOOL                afs_toc_data_reels_load_string(afs_toc_data_reels * toc_data_reels, const char * in);
DBOOL                afs_toc_data_reels_load_xml(afs_toc_data_reels * toc_data_reels, struct mxml_node_s* node);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TOC_DATA_REELS_H
