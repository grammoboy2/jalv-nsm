/*
  Copyright 2008-2011 David Robillard <http://drobilla.net>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef LV2_EVBUF_H
#define LV2_EVBUF_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Format of actual buffer.
*/
typedef enum {
	/**
	   An old ev:EventBuffer (LV2_Event_Buffer).
	*/
	LV2_EVBUF_EVENT,

	/**
	   A new atom:EventBuffer (LV2_Atom_Event_Buffer).
	*/
	LV2_EVBUF_ATOM
} LV2_Evbuf_Type;

/**
   An abstract/opaque LV2 event buffer.
*/
typedef struct LV2_Evbuf_Impl LV2_Evbuf;

/**
   An iterator over an LV2_Evbuf.
*/
typedef struct {
	LV2_Evbuf* evbuf;
	uint32_t   offset;
} LV2_Evbuf_Iterator;

/**
   Allocate a new, empty event buffer.
*/
LV2_Evbuf*
lv2_evbuf_new(uint32_t capacity, LV2_Evbuf_Type type);

/**
   Free an event buffer allocated with lv2_evbuf_new.
*/
void
lv2_evbuf_free(LV2_Evbuf* evbuf);

/**
   Clear and initialize an existing event buffer.
   The contents of buf are ignored entirely and overwritten, except capacity
   which is unmodified.
*/
void
lv2_evbuf_reset(LV2_Evbuf* evbuf);

/**
   Return the total padded size of the events stored in the buffer.
*/
uint32_t
lv2_evbuf_get_size(LV2_Evbuf* evbuf);

/**
   Return the number of events stored in the buffer.
*/
uint32_t
lv2_evbuf_get_event_count(LV2_Evbuf* evbuf);

/**
   Return the actual buffer implementation.
   The format of the buffer returned depends on the buffer type.
*/
void*
lv2_evbuf_get_buffer(LV2_Evbuf* evbuf);

/**
   Reset an iterator to point to the start of @a buf.
   @return True if @a iter is valid, otherwise false (buffer is empty)
*/
LV2_Evbuf_Iterator
lv2_evbuf_begin(LV2_Evbuf* evbuf);

/**
   Check if @a iter is valid.
   @return True if @a iter is valid, otherwise false (past end of buffer)
*/
bool
lv2_evbuf_is_valid(LV2_Evbuf_Iterator iter);

/**
   Advance @a iter forward one event.
   @a iter must be valid.
   @return True if @a iter is valid, otherwise false (reached end of buffer)
*/
LV2_Evbuf_Iterator
lv2_evbuf_next(LV2_Evbuf_Iterator iter);

/**
   Dereference an event iterator (i.e. get the event currently pointed to).
   @a iter must be valid.
   @a type Set to the type of the event.
   @a size Set to the size of the event.
   @a data Set to the contents of the event.
   @return True on success.
*/
bool
lv2_evbuf_get(LV2_Evbuf_Iterator iter,
              uint32_t*          frames,
              uint32_t*          subframes,
              uint32_t*          type,
              uint32_t*          size,
              uint8_t**          data);

/**
   Write an event at @a iter.
   The event (if any) pointed to by @a iter will be overwritten, and @a iter
   incremented to point to the following event (i.e. several calls to this
   function can be done in sequence without twiddling iter in-between).
   @return True if event was written, otherwise false (buffer is full).
*/
bool
lv2_evbuf_write(LV2_Evbuf_Iterator* iter,
                uint32_t            frames,
                uint32_t            subframes,
                uint32_t            type,
                uint32_t            size,
                const uint8_t*      data);

#ifdef __cplusplus
}
#endif

#endif /* LV2_EVBUF_H */