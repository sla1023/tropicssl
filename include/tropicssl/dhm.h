/**
 * \file dhm.h
 *
 *  Based on XySSL: Copyright (C) 2006-2008  Christophe Devine
 *
 *  Copyright (C) 2009  Paul Bakker <polarssl_maintainer at polarssl dot org>
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of PolarSSL or XySSL nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef TROPICSSL_DHM_H
#define TROPICSSL_DHM_H

#include "tropicssl/config.h"

#if defined(TROPICSSL_DHM)
#include "tropicssl/bignum.h"

typedef struct {
	size_t len;		/*!<  size(P) in chars  */
	mpi P;			/*!<  prime modulus     */
	mpi G;			/*!<  generator         */
	mpi X;			/*!<  secret value      */
	mpi GX;			/*!<  self = G^X mod P  */
	mpi GY;			/*!<  peer = G^Y mod P  */
	mpi K;			/*!<  key = GY^X mod P  */
	mpi RP;			/*!<  cached R^2 mod P  */
} dhm_context;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * \brief          Parse the ServerKeyExchange parameters
	 *
	 * \param ctx      DHM context
	 * \param p        &(start of input buffer)
	 * \param end      end of buffer
	 *
	 * \return         0 if successful, or an TROPICSSL_ERR_DHM_XXX error code
	 */
	int dhm_read_params(dhm_context * ctx,
			    uint8_t **p, const uint8_t *end);

	/**
	 * \brief          Setup and write the ServerKeyExchange parameters
	 *
	 * \param ctx      DHM context
	 * \param x_size   private value size in bits
	 * \param output   destination buffer
	 * \param olen     number of chars written
	 * \param f_rng    RNG function
	 * \param p_rng    RNG parameter
	 *
	 * \note           This function assumes that ctx->P and ctx->G
	 *                 have already been properly set (for example
	 *                 using mpi_read_string or mpi_read_binary).
	 *
	 * \return         0 if successful, or an TROPICSSL_ERR_DHM_XXX error code
	 */
	int dhm_make_params(dhm_context * ctx, int x_size,
			    uint8_t *output, size_t *olen,
			    int (*f_rng) (void *), void *p_rng);

	/**
	 * \brief          Import the peer's public value G^Y
	 *
	 * \param ctx      DHM context
	 * \param input    input buffer
	 * \param ilen     size of buffer
	 *
	 * \return         0 if successful, or an TROPICSSL_ERR_DHM_XXX error code
	 */
	int dhm_read_public(dhm_context * ctx, const uint8_t *input, size_t ilen);

	/**
	 * \brief          Create own private value X and export G^X
	 *
	 * \param ctx      DHM context
	 * \param x_size   private value size in bits
	 * \param output   destination buffer
	 * \param olen     must be equal to ctx->P.len
	 * \param f_rng    RNG function
	 * \param p_rng    RNG parameter
	 *
	 * \return         0 if successful, or an TROPICSSL_ERR_DHM_XXX error code
	 */
	int dhm_make_public(dhm_context * ctx, int s_size,
			    uint8_t *output, size_t olen,
			    int (*f_rng) (void *), void *p_rng);

	/**
	 * \brief          Derive and export the shared secret (G^Y)^X mod P
	 *
	 * \param ctx      DHM context
	 * \param output   destination buffer
	 * \param olen     number of chars written
	 *
	 * \return         0 if successful, or an TROPICSSL_ERR_DHM_XXX error code
	 */
	int dhm_calc_secret(dhm_context * ctx,
			    uint8_t *output, size_t *olen);

	/*
	 * \brief          Free the components of a DHM key
	 */
	void dhm_free(dhm_context * ctx);

#if defined(TROPICSSL_SELF_TEST)
	/**
	 * \brief          Checkup routine
	 *
	 * \return         0 if successful, or 1 if the test failed
	 */
	int dhm_self_test(int verbose);
#endif

#ifdef __cplusplus
}
#endif

#endif              /* TROPICSSL_DHM */
#endif
