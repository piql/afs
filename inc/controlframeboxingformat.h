#ifdef __cplusplus
extern "C" {
#endif


#define AFS_FRAME_FORMAT_V5 4
#define AFS_FRAME_FORMAT_V4 3
#define AFS_FRAME_FORMAT_V3 2
#define AFS_FRAME_FORMAT_V2 1
#define AFS_FRAME_FORMAT_V1 0
#define AFS_FRAME_FORMAT_MAX 5
extern unsigned char* afs_control_frame_format[AFS_FRAME_FORMAT_MAX];
extern const unsigned int afs_control_frame_format_len[AFS_FRAME_FORMAT_MAX];

#ifdef __cplusplus
}
#endif

