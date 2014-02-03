/*
 * Copyright (C) 2014 Philippe Gerum <rpm@xenomai.org>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */
#ifndef _COPPERPLATE_REGISTRY_OBSTACK_H
#define _COPPERPLATE_REGISTRY_OBSTACK_H

#ifdef CONFIG_XENO_REGISTRY

#include <boilerplate/obstack.h>
#include <copperplate/registry.h>
#include <copperplate/heapobj.h>

/*
 * Assume we may want fast allocation of private memory from real-time
 * mode when growing the obstack.
 */
#define obstack_chunk_alloc	pvmalloc
#define obstack_chunk_free	pvfree

struct fsobstack {
	struct obstack obstack;
	void *data;
	size_t len;
};

struct fsobstack_syncops {
	int (*prepare_cache)(struct fsobstack *o,
			     struct obstack *cache, int item_count);
	size_t (*collect_data)(void *p, struct threadobj *thobj);
	size_t (*format_data)(struct fsobstack *o, void *p);
};

struct syncobj;

#ifdef __cplusplus
extern "C" {
#endif

int fsobstack_grow_format(struct fsobstack *o,
			  const char *fmt, ...);

int fsobstack_grow_syncobj_grant(struct fsobstack *o,
				 struct syncobj *sobj,
				 struct fsobstack_syncops *ops);

int fsobstack_grow_syncobj_drain(struct fsobstack *o,
				 struct syncobj *sobj,
				 struct fsobstack_syncops *ops);

ssize_t fsobstack_pull(struct fsobstack *o,
		       char *buf, size_t size);

ssize_t fsobj_obstack_read(struct fsobj *fsobj,
			   char *buf, size_t size, off_t offset,
			   void *priv);

int fsobj_obstack_release(struct fsobj *fsobj, void *priv);

#ifdef __cplusplus
}
#endif

static inline void fsobstack_init(struct fsobstack *o)
{
	obstack_init(&o->obstack);
	o->data = NULL;
	o->len = 0;
}

static inline void fsobstack_destroy(struct fsobstack *o)
{
	obstack_free(&o->obstack, NULL);
}

static inline void fsobstack_finish(struct fsobstack *o)
{
	o->len = obstack_object_size(&o->obstack);
	o->data = obstack_finish(&o->obstack);
}

static inline
void registry_init_file_obstack(struct fsobj *fsobj, 
				const struct registry_operations *ops)
{
	registry_init_file(fsobj, ops, sizeof(struct fsobstack));
}

#else /* !CONFIG_XENO_REGISTRY */

static inline
void registry_init_file_obstack(struct fsobj *fsobj, 
				const struct registry_operations *ops)
{ }

#endif /* !CONFIG_XENO_REGISTRY */

#endif /* !_COPPERPLATE_REGISTRY_OBSTACK_H */
