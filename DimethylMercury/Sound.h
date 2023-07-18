#include <Windows.h>
#include <math.h>
#pragma comment(lib, "winmm.lib")
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