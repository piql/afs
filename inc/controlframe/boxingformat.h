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
#include "mxml.h"

// Definition structure afs_boxing_format
typedef struct afs_boxing_format_s
{
    boxing_config * config;
} afs_boxing_format;

afs_boxing_format* afs_boxing_format_create();
afs_boxing_format* afs_boxing_format_create2(const boxing_config*);
void               afs_boxing_format_init(afs_boxing_format*);
void               afs_boxing_format_init2(afs_boxing_format*, const boxing_config*);
void               afs_boxing_format_free(afs_boxing_format*);

DBOOL              afs_boxing_format_save_xml(mxml_node_t * out, afs_boxing_format* boxing_format);
char *             afs_boxing_format_save_string(afs_boxing_format* boxing_format, DBOOL compact);
DBOOL              afs_boxing_format_load_xml(afs_boxing_format* boxing_format, mxml_node_t * in);
DBOOL              afs_boxing_format_load_string(afs_boxing_format* boxing_format, const char * in);

#if !defined( AFS_DISABLE_FILE_IO )
DBOOL              afs_boxing_format_save_config_file(const char * file_name, afs_boxing_format* boxing_format, DBOOL compact);
DBOOL              afs_boxing_format_load_config_file(afs_boxing_format* boxing_format, const char * file_name);
#endif

const char*        afs_boxing_format_get_name(afs_boxing_format* format);
unsigned int       afs_boxing_format_get_scaling_factor(afs_boxing_format* format);
unsigned int       afs_boxing_format_get_width(afs_boxing_format* format);
unsigned int       afs_boxing_format_get_height(afs_boxing_format* format);
unsigned int       afs_boxing_format_get_data_frame_width(afs_boxing_format* format);
unsigned int       afs_boxing_format_get_data_frame_height(afs_boxing_format* format);
unsigned int       afs_boxing_format_get_bits_per_pixel(afs_boxing_format* format);
unsigned int       afs_boxing_format_get_symbols_per_pixel(afs_boxing_format* format);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
