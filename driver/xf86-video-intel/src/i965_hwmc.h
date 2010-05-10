#define I965_MC_STATIC_BUFFER_SIZE	(1024*512)
#define I965_MAX_SURFACES		12
struct _i830_memory;
struct drm_memory_block {
	struct _i830_memory *buffer;
	drm_handle_t handle;
	drmAddress ptr;
	size_t size;
	unsigned long offset;
};

struct i965_xvmc_surface {
	int w, h;
	unsigned int no;
	void *handle;
	dri_bo *bo;
};

struct i965_xvmc_context {
	struct _intel_xvmc_common comm;
	struct drm_memory_block static_buffer;
	struct drm_memory_block blocks;
	struct drm_memory_block slice;
	struct i965_xvmc_surface *surfaces[I965_MAX_SURFACES];
	unsigned int is_g4x:1;
	unsigned int is_965_q:1;
	unsigned int is_igdng:1;
};
