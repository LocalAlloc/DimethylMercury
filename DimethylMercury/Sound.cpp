#include "Sound.h"

#define SYNTH_LENGTH 16
#define FAST_PI 3.14159265358979323846f

typedef VOID(WINAPI* AUDIOSEQUENCE_OPERATION)(_In_ INT nSamplesPerSec);
typedef VOID(WINAPI* AUDIO_SEQUENCE)(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
typedef struct _AUDIO_SEQUENCE_PARAMS
{
	INT nSamplesPerSec;
	INT nSampleCount;
	PSHORT psSamples;
	AUDIOSEQUENCE_OPERATION pPreAudioOp;
	AUDIOSEQUENCE_OPERATION pPostAudioOp;
	AUDIO_SEQUENCE pAudioSequence;
} AUDIO_SEQUENCE_PARAMS, * PAUDIO_SEQUENCE_PARAMS;

// Define a new type for the audio buffer
typedef SHORT* PAUDIO_BUFFER;

typedef VOID(WINAPI* AUDIO_SEQUENCE)(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PAUDIO_BUFFER pAudioBuffer);

VOID WINAPI AudioPayloadThread(VOID);
VOID WINAPI AudioSequenceThread(_In_ PAUDIO_SEQUENCE_PARAMS pAudioParams);
VOID WINAPI ExecuteAudioSequence(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _In_ AUDIO_SEQUENCE pAudioSequence,
	_In_opt_ AUDIOSEQUENCE_OPERATION pPreAudioOp,
	_In_opt_ AUDIOSEQUENCE_OPERATION pPostAudioOp);

VOID WINAPI AudioSequence1(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence2(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence3(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence4(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence5(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence6(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence7(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence8(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence9(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence10(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence11(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence12(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence13(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence14(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence15(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence16(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
VOID WINAPI AudioSequence17(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples);
// Random number generator using xorshift algorithm
DWORD g_dwRandSeed = 0xdeadbeef;
inline DWORD Xorshift32()
{
	DWORD x = g_dwRandSeed;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	g_dwRandSeed = x;
	return x;
}
// Fast sine function using approximation
inline FLOAT FastSine(FLOAT x)
{
	x *= 0.15915494309189535f; // 1/FAST_PI
	x -= (INT)x;
	FLOAT y = x * (3.14159265358979323846f * 2);
	if (x < 0.5f)
	{
		return 4 * x * (1 - x) * y;
	}
	else
	{
		return sinf(y) - 4 * (x - 1) * x * y;
	}
}
// Fast cosine function using approximation
inline FLOAT FastCosine(FLOAT x)
{
	// Shift the input value to be within [0, 1] range
	x = fmodf(x, 1.0f);
	if (x < 0.0f)
		x += 1.0f;

	// Calculate the cosine using the fast sine function
	return FastSine(x + 0.25f);
}
// Triangle wave function
inline FLOAT TriangleWave(INT t, INT nFreq, FLOAT fSine)
{
	FLOAT x = (FLOAT)t / 10.f;
	FLOAT freq = (FLOAT)nFreq;
	x -= (INT)x;
	x = (x < 0.5f) ? (4 * x - 1) : (3 - 4 * x);
	return x * fSine / freq;
}
// Square wave function
inline FLOAT SquareWave(INT t, INT nFreq, INT nSampleCount)
{
	FLOAT x = (FLOAT)t / 10.f;
	FLOAT freq = (FLOAT)nFreq;
	x -= (INT)x;
	return (x < 0.5f) ? (freq / nSampleCount) : (-freq / nSampleCount);
}
// Sawtooth wave function
inline FLOAT SawtoothWave(INT t, INT nFreq, INT nSampleCount)
{
	FLOAT x = (FLOAT)t / 10.f;
	FLOAT freq = (FLOAT)nFreq;
	x -= (INT)x;
	return (2 * x - 1) * freq / nSampleCount;
}
// Sine wave function
inline FLOAT SineWave(INT t, INT nFreq, INT nSampleCount)
{
	FLOAT x = (FLOAT)t / 10.f;
	FLOAT freq = (FLOAT)nFreq;
	x -= (INT)x;
	return sinf(x * 2 * FAST_PI) * freq / nSampleCount;
}
VOID WINAPI AudioPayloadThread(VOID)
{
	for (;;)
	{
		INT piOrder[SYNTH_LENGTH];
		INT nRandIndex;
		INT nNumber;

		for (INT i = 0; i < SYNTH_LENGTH; i++)
		{
			piOrder[i] = i;
		}

		for (INT i = 0; i < SYNTH_LENGTH; i++)
		{
			nRandIndex = Xorshift32() % 16;
			nNumber = piOrder[nRandIndex];
			piOrder[nRandIndex] = piOrder[i];
			piOrder[i] = nNumber;
		}

		// Execute AudioSequence1
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence1, NULL, NULL);

		// Execute AudioSequence2
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence2, NULL, NULL);

		// Execute AudioSequence3
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence3, NULL, NULL);

		// Execute AudioSequence4
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence4, NULL, NULL);
		// Execute AudioSequence5
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence5, NULL, NULL);

		// Execute AudioSequence6
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence6, NULL, NULL);

		// Execute AudioSequence7
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence7, NULL, NULL);
		// Execute AudioSequence8
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence8, NULL, NULL);
		// Execute AudioSequence9
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence9, NULL, NULL);

		// Execute AudioSequence10
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence10, NULL, NULL);

		// Execute AudioSequence11
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence11, NULL, NULL);
		// Execute AudioSequence12
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence12, NULL, NULL);

		// Execute AudioSequence13
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence13, NULL, NULL);

		// Execute AudioSequence14
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence14, NULL, NULL);


		// Execute AudioSequence15
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence15, NULL, NULL);

		// Execute AudioSequence16
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence16, NULL, NULL);
		// Execute FinalAudioSequence
		ExecuteAudioSequence(44100, 44100 * 4, AudioSequence17, NULL, NULL);
	}
}
VOID WINAPI AudioSequenceThread(_In_ PAUDIO_SEQUENCE_PARAMS pAudioParams)
{
	ExecuteAudioSequence(pAudioParams->nSamplesPerSec,
		pAudioParams->nSampleCount,
		pAudioParams->pAudioSequence,
		pAudioParams->pPreAudioOp,
		pAudioParams->pPostAudioOp);
}
VOID WINAPI ExecuteAudioSequence(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _In_ AUDIO_SEQUENCE pAudioSequence,
	_In_opt_ AUDIOSEQUENCE_OPERATION pPreAudioOp,
	_In_opt_ AUDIOSEQUENCE_OPERATION pPostAudioOp)
{
	HANDLE hHeap = GetProcessHeap();
	PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount * 2);
	WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, nSamplesPerSec, nSamplesPerSec * 2, 2, 16, 0 };
	WAVEHDR waveHdr = { (PCHAR)psSamples, nSampleCount * 2, 0, 0, 0, 0, NULL, 0 };
	HWAVEOUT hWaveOut;
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);

	if (pPreAudioOp)
	{
		pPreAudioOp(nSamplesPerSec);
	}

	pAudioSequence(nSamplesPerSec, nSampleCount, psSamples);

	if (pPostAudioOp)
	{
		pPostAudioOp(nSamplesPerSec);
	}

	waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
	waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));

	Sleep(nSampleCount * 1000 / nSamplesPerSec);

	while (!(waveHdr.dwFlags & WHDR_DONE))
	{
		Sleep(1);
	}

	waveOutReset(hWaveOut);
	waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
	HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI AudioSequence1(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	for (INT t = 0; t < nSampleCount; t++)
	{
		INT nFreq = (INT)(FastSine((FLOAT)t / (1000.f - t / (nSampleCount / 1000))) * 100.f + 500.f);
		psSamples[t] = (SHORT)(SquareWave(t, nFreq, nSampleCount) * (FLOAT)SHRT_MAX * .1f);
	}
}
VOID WINAPI AudioSequence2(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	for (INT t = 0; t < nSampleCount * 2; t++)
	{
		BYTE bFreq = (BYTE)((t | t % 255 | t % 257) + (t & t >> 8) + (t * (42 & t >> 10)) + ((t % ((t >> 8 | t >> 16) + 1)) ^ t));
		((BYTE*)psSamples)[t] = bFreq;
	}
}
VOID WINAPI AudioSequence3(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	INT nCubeRoot = (INT)cbrtf((FLOAT)nSampleCount) + 1;
	for (INT z = 0; z < nCubeRoot; z++)
	{
		for (INT y = 0; y < nCubeRoot; y++)
		{
			for (INT x = 0; x < nCubeRoot; x++)
			{
				INT nIndex = z * nCubeRoot * nCubeRoot + y * nCubeRoot + x;
				if (nIndex >= nSampleCount)
					continue;

				INT nFreq = (INT)((FLOAT)(y & z & x) * FastSine((FLOAT)(z * y * x) / 100.f));
				psSamples[nIndex] =
					(SHORT)(SquareWave(y + z * x, (nFreq % 500) + 1000, nSamplesPerSec) * (FLOAT)SHRT_MAX * .3f) +
					(SHORT)(SawtoothWave(x | z, (150 - (nFreq % 200) / 4) + 800, nSamplesPerSec) * (FLOAT)SHRT_MAX * .3f) +
					(SHORT)(TriangleWave((FLOAT)(x & y & z) + (SquareWave(x + y, nFreq % 50, nSamplesPerSec) * nSamplesPerSec),
						(nFreq % 50) / 10 + 50, nSamplesPerSec) * (FLOAT)SHRT_MAX * .3f);
			}
		}
	}
}
VOID WINAPI AudioSequence4(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	for (INT t = 0; t < nSampleCount; t++)
	{
		INT nFreq = (INT)(FastSine((FLOAT)t / (1000.f - t / (nSampleCount / 1000))) * 100.f + 500.f);
		psSamples[t] = (SHORT)(SquareWave(t, nFreq, nSampleCount) * (FLOAT)SHRT_MAX * .1f);
	}
}
VOID WINAPI AudioSequence5(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	for (INT t = 0; t < nSampleCount; t++)
	{
		INT nFreq = (INT)(FastSine((FLOAT)t / (1000.f - t / (nSampleCount / 1000))) * 100.f + 500.f);
		psSamples[t] = (SHORT)(TriangleWave(t, nFreq, FastSine((FLOAT)t / 1000.f)) * (FLOAT)SHRT_MAX * .2f);
	}
}
VOID WINAPI AudioSequence6(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	for (INT t = 0; t < nSampleCount; t++)
	{
		INT nFreq = (INT)(FastSine((FLOAT)t / (1000.f - t / (nSampleCount / 1000))) * 100.f + 500.f);
		psSamples[t] = (SHORT)(SineWave(t, nFreq, nSampleCount) * (FLOAT)SHRT_MAX * .2f);
	}
}
VOID WINAPI AudioSequence7(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	for (INT t = 0; t < nSampleCount * 2; t++)
	{
		BYTE bFreq = (BYTE)(((t >> 12 & t >> 8) >> (t >> 20 & t >> 12)) * t);
		((BYTE*)psSamples)[t] = bFreq;
	}
}
VOID WINAPI AudioSequence8(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	SHORT sRand = (SHORT)Xorshift32();
	for (INT t = 0; t < nSampleCount; t++)
	{
		INT nRand = (nSampleCount - t * 2) / 512;
		if (nRand < 24)
			nRand = 24;
		if (!(Xorshift32() % nRand))
		{
			sRand = (SHORT)Xorshift32();
		}
		psSamples[t] = (SHORT)(SawtoothWave(t, sRand, nSampleCount) * (FLOAT)SHRT_MAX * .1f)
			& ~sRand | ((SHORT)Xorshift32() >> 12) +
			(SHORT)(SineWave(Xorshift32() % nSampleCount, nRand ^ sRand, nSampleCount) * (FLOAT)SHRT_MAX * .1f) +
			(SHORT)(TriangleWave(t, 3000, nSampleCount) * (FLOAT)SHRT_MAX);
	}
}
VOID WINAPI AudioSequence9(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	INT nSquareRoot = (INT)sqrtf((FLOAT)nSampleCount) + 1;
	for (INT y = 0; y < nSquareRoot; y++)
	{
		for (INT x = 0; x < nSquareRoot; x++)
		{
			INT nIndex = y * nSquareRoot + x;
			if (nIndex >= nSampleCount)
				continue;

			INT nFreq = (INT)((FLOAT)(y | x) * FastSine((FLOAT)(y * x) / 1000.f));
			psSamples[nIndex] =
				(SHORT)(SquareWave(y & x, (nFreq % 500) + 1000, nSamplesPerSec) * (FLOAT)SHRT_MAX * .3f);
		}
	}
}
VOID WINAPI AudioSequence10(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	for (INT t = 0; t < nSampleCount * 2; t++)
	{
		//Most Detailed I Hate It!
		// Calculate the frequency using bitwise operations and basic arithmetic
		// to create a byte value that represents the frequency.
		// The calculation is divided into smaller parts for readability.

		// Part 1: Calculate the frequency components using bitwise operations
		BYTE bFreq = (BYTE)((t << ((t >> 1 | t >> 8) ^ (t >> 13)))        // Left shift
			| (t >> 8 & t >> 16) * t >> 4);                              // Right shift

		// Part 2: Add components using basic arithmetic operations
		bFreq += (t * (t >> 7 | t >> 10)) >> (t >> 18 & t);               // Addition
		bFreq += (t * t) / ((t ^ t >> 12) + 1);                          // Division

		// Part 3: Ternary operator to conditionally add or subtract a value
		FLOAT w = powf(2.f, (FLOAT)(t >> 8 & t >> 13));
		bFreq += (((128 / ((BYTE)w + 1)) & t) > 1 ? (BYTE)w * t : -(BYTE)w * (t + 1));

		// Store the final frequency value in the audio sample buffer
		((BYTE*)psSamples)[t] = bFreq;
	}
}
VOID WINAPI AudioSequence11(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	for (INT t = 0; t < nSampleCount * 2; t++)
	{
		BYTE bFreq = (BYTE)(((t * ((t >> 8 & t >> 3) >> (t >> 16 & t))) +
			((t * (t >> 8 & t >> 3)) >> (t >> 16 & t))));
		((BYTE*)psSamples)[t] = bFreq;
	}
}
VOID WINAPI AudioSequence12(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	for (INT t = 0; t < nSampleCount; t++)
	{
		psSamples[t] = (SHORT)(TriangleWave(__rdtsc() % 8, 1500, nSampleCount) * (FLOAT)SHRT_MAX * .3f) |
			(SHORT)(SquareWave(__rdtsc() % 8, 1000, nSampleCount) * (FLOAT)SHRT_MAX * .3f) +
			(SHORT)~t + ((SHORT)t >> 2);
	}
}
VOID WINAPI AudioSequence13(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	for (INT t = 0; t < nSampleCount; t++)
	{
		psSamples[t] = (SHORT)(SawtoothWave(__rdtsc() % 1500, 1500, nSampleCount) * (FLOAT)SHRT_MAX * .3f) ^
			(((SHORT)(SawtoothWave(t % 10, t % 1000, nSampleCount) * (FLOAT)SHRT_MAX * .1f)) >> 8);
	}
}
VOID WINAPI AudioSequence14(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	INT nSquareRoot = (INT)sqrtf((FLOAT)nSampleCount) + 1;
	for (INT y = 0; y < nSquareRoot; y++)
	{
		for (INT x = 0; x < nSquareRoot; x++)
		{
			INT nIndex = y * nSquareRoot + x;
			if (nIndex >= nSampleCount)
				continue;

			INT nFreq = (INT)((FLOAT)(y | x) * FastCosine((FLOAT)(y & x) / 10.f));
			psSamples[nIndex] = (SHORT)(SineWave(y + x, (nFreq % 1000) + 1000, nSamplesPerSec) * (FLOAT)SHRT_MAX * .3f);
		}
	}
}
VOID WINAPI AudioSequence15(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	INT nSquareRoot = (INT)sqrtf((FLOAT)nSampleCount) + 1;
	for (INT y = 0; y < nSquareRoot; y++)
	{
		for (INT x = 0; x < nSquareRoot; x++)
		{
			INT nIndex = y * nSquareRoot + x;
			if (nIndex >= nSampleCount)
				continue;

			INT nFreq = (INT)((FLOAT)(y | x) * FastCosine((FLOAT)(y & x) / 10.f));
			psSamples[nIndex] =
				(SHORT)(SquareWave(y + x, (nFreq % 500) + 1000, nSamplesPerSec) * (FLOAT)SHRT_MAX * .3f) +
				(SHORT)(TriangleWave(x, 1500, nSamplesPerSec) * (FLOAT)SHRT_MAX * .2f);
		}
	}
}
VOID WINAPI AudioSequence16(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	INT nSquareRoot = (INT)sqrtf((FLOAT)nSampleCount) + 1;
	for (INT y = 0; y < nSquareRoot; y++)
	{
		for (INT x = 0; x < nSquareRoot; x++)
		{
			INT nIndex = y * nSquareRoot + x;
			if (nIndex >= nSampleCount)
				continue;

			INT nFreq = (INT)((FLOAT)(y | x) * FastCosine((FLOAT)(y & x) / 10.f));
			psSamples[nIndex] =
				(SHORT)(SineWave(y + x, (nFreq % 1000) + 1000, nSamplesPerSec) * (FLOAT)SHRT_MAX * .3f) +
				(SHORT)(SquareWave(x, 1500, nSamplesPerSec) * (FLOAT)SHRT_MAX * .3f);
		}
	}
}
VOID WINAPI AudioSequence17(_In_ INT nSamplesPerSec, _In_ INT nSampleCount, _Inout_ PSHORT psSamples)
{
	UNREFERENCED_PARAMETER(nSamplesPerSec);

	INT nSquareRoot = (INT)sqrtf((FLOAT)nSampleCount) + 1;
	for (INT y = 0; y < nSquareRoot; y++)
	{
		for (INT x = 0; x < nSquareRoot; x++)
		{
			INT nIndex = y * nSquareRoot + x;
			if (nIndex >= nSampleCount)
				continue;

			INT nFreq = (INT)((FLOAT)(y | x) * FastCosine((FLOAT)(y & x) / 10.f));
			psSamples[nIndex] =
				(SHORT)(SineWave(y + x, (nFreq % 1000) + 1000, nSamplesPerSec) * (FLOAT)SHRT_MAX * .3f) +
				(SHORT)(SquareWave(x, 1500, nSamplesPerSec) * (FLOAT)SHRT_MAX * .3f) +
				(SHORT)(TriangleWave(x + y, 2000, nSamplesPerSec) * (FLOAT)SHRT_MAX * .2f);
		}
	}
}