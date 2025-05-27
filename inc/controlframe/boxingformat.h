#ifndef BOXING_FORMAT_H
#define BOXING_FORMAT_H

/*****************************************************************************
**
**  Definition of the control frame boxing format interface
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

#ifdef __cplusplus
extern "C" {
#endif

#include "boxing/config.h"
#include "boxing/graphics/genericframe.h"

struct mxml_node_s;

    // Definition structure afs_boxing_format
typedef struct afs_boxing_format_s
{
    boxing_config * config;

    const char * name;
    unsigned int bytes_per_frame;
    unsigned int data_bytes_per_frame;
    unsigned int data_stripe_size;
    unsigned int scaling_factor;
    unsigned int width;
    unsigned int height;
    unsigned int data_frame_width;
    unsigned int data_frame_height;
    unsigned int bits_per_pixel;
    unsigned int symbol_per_pixel;

    int reference_count;
} afs_boxing_format;

afs_boxing_format * afs_boxing_format_create(void);
afs_boxing_format * afs_boxing_format_create2(const boxing_config *);
void                afs_boxing_format_init(afs_boxing_format *boxing_format);
void                afs_boxing_format_init2(afs_boxing_format *boxing_format, const boxing_config *config);
void                afs_boxing_format_free(afs_boxing_format *);

afs_boxing_format * afs_boxing_format_clone(const afs_boxing_format * boxing_format);
afs_boxing_format * afs_boxing_format_get_new_reference(afs_boxing_format * boxing_format);
DBOOL               afs_boxing_format_equal(const afs_boxing_format * boxing_format1, const afs_boxing_format * boxing_format2);

unsigned int        afs_boxing_format_get_data_frames(const afs_boxing_format * boxing_format, const size_t file_size);
void                afs_boxing_format_set_config(afs_boxing_format * boxing_format, const boxing_config * config);

DBOOL               afs_boxing_format_save_xml(struct mxml_node_s * out, afs_boxing_format * boxing_format);
char *              afs_boxing_format_save_string(afs_boxing_format * boxing_format, DBOOL compact);
DBOOL               afs_boxing_format_load_xml(afs_boxing_format * boxing_format, struct mxml_node_s * in);
DBOOL               afs_boxing_format_load_string(afs_boxing_format * boxing_format, const char * in);

#if !defined( AFS_DISABLE_FILE_IO )
DBOOL               afs_boxing_format_save_config_file(const char * file_name, afs_boxing_format* boxing_format, DBOOL compact);
DBOOL               afs_boxing_format_load_config_file(afs_boxing_format * boxing_format, const char * file_name);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
