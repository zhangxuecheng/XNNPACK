// Auto-generated file. Do not edit!
//   Template: src/f16-hswish/neonfp16arith.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <arm_neon.h>

#include <xnnpack/common.h>
#include <xnnpack/hswish.h>


void xnn_f16_hswish_ukernel__neonfp16arith_x16(
    size_t n,
    const void* restrict x_ptr,
    void* restrict y_ptr,
    const struct xnn_f16_hswish_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_DISABLE_TSAN
{
  assert(n != 0);
  assert(n % sizeof(__fp16) == 0);

  const __fp16* x = (const __fp16*) x_ptr;
  __fp16* y = (__fp16*) y_ptr;

  const float16x8_t vsixth = vld1q_dup_f16(&params->sixth);
  const float16x8_t vhalf = vld1q_dup_f16(&params->half);
  const float16x8_t vone = vld1q_dup_f16(&params->one);
  const float16x8_t vzero = vdupq_n_f16(0);

  for (; n >= 16 * sizeof(__fp16); n -= 16 * sizeof(__fp16)) {
    const float16x8_t vx01234567 = vld1q_f16(x); x += 8;
    const float16x8_t vx89ABCDEF = vld1q_f16(x); x += 8;

    float16x8_t vacc01234567 = vfmaq_f16(vhalf, vx01234567, vsixth);
    float16x8_t vacc89ABCDEF = vfmaq_f16(vhalf, vx89ABCDEF, vsixth);

    vacc01234567 = vmaxq_f16(vacc01234567, vzero);
    vacc89ABCDEF = vmaxq_f16(vacc89ABCDEF, vzero);

    vacc01234567 = vminq_f16(vacc01234567, vone);
    vacc89ABCDEF = vminq_f16(vacc89ABCDEF, vone);

    vacc01234567 = vmulq_f16(vacc01234567, vx01234567);
    vacc89ABCDEF = vmulq_f16(vacc89ABCDEF, vx89ABCDEF);

    vst1q_f16(y, vacc01234567); y += 8;
    vst1q_f16(y, vacc89ABCDEF); y += 8;
  }
  for (; n >= 8 * sizeof(__fp16); n -= 8 * sizeof(__fp16)) {
    const float16x8_t vx1234567 = vld1q_f16(x); x += 8;
    float16x8_t vacc01234567 = vfmaq_f16(vhalf, vx1234567, vsixth);
    vacc01234567 = vmaxq_f16(vacc01234567, vzero);
    vacc01234567 = vminq_f16(vacc01234567, vone);
    vacc01234567 = vmulq_f16(vacc01234567, vx1234567);
    vst1q_f16(y, vacc01234567); y += 8;
  }
  if XNN_UNLIKELY(n != 0) {
    const float16x8_t vx1234567 = vld1q_f16(x);
    float16x8_t vacc01234567 = vfmaq_f16(vhalf, vx1234567, vsixth);
    vacc01234567 = vmaxq_f16(vacc01234567, vzero);
    vacc01234567 = vminq_f16(vacc01234567, vone);
    vacc01234567 = vmulq_f16(vacc01234567, vx1234567);

    float16x4_t vacc0123 = vget_low_f16(vacc01234567);
    if (n & (4 * sizeof(__fp16))) {
      vst1_f16(y, vacc0123); y += 4;
      vacc0123 = vget_high_f16(vacc01234567);
    }

    if (n & (2 * sizeof(__fp16))) {
      vst1_lane_u32(__builtin_assume_aligned(y, 1), vreinterpret_u32_f16(vacc0123), 0); y += 2;
      vacc0123 = vext_f16(vacc0123, vacc0123, 2);
    }

    if (n & (1 * sizeof(__fp16))) {
      vst1_lane_f16(y, vacc0123, 0);
    }
  }
}
