#include <windows.h>
#include <algorithm>
#include <exedit.hpp>

static char name[] = "音声クリッピング";

constexpr int track_n = 1;
static char* track_name[track_n] = { const_cast<char*>("レベル") };


static int track_default[track_n] = { 4096 };
static int track_s[track_n] = { 0 };
static int track_e[track_n] = { 32767 };

constexpr int check_n = 0;


BOOL func_proc(ExEdit::Filter* efp, ExEdit::FilterProcInfo* efpip) {
    short* audiop;
    if (has_flag(efp->flag, ExEdit::Filter::Flag::Effect)) {
        audiop = efpip->audio_data;
    } else {
        audiop = efpip->audio_p;
    }
    short level = std::clamp(efp->track[0], 0, SHRT_MAX);
    short neg_level = ~level;
    for (int i = efpip->audio_n * efpip->audio_ch; 0 < i; i--) {
        *audiop = std::clamp(*audiop, neg_level, level);
        audiop++;
    }
    
    return TRUE;
}

ExEdit::Filter effect_ef = {
    .flag = ExEdit::Filter::Flag::Audio | ExEdit::Filter::Flag::Effect,
    .name = name,
    .track_n = track_n,
    .track_name = track_name,
    .track_default = track_default,
    .track_s = track_s,
    .track_e = track_e,
    .check_n = check_n,
    .func_proc = func_proc,
};
ExEdit::Filter filter_ef = {
    .flag = ExEdit::Filter::Flag::Audio,
    .name = name,
    .track_n = track_n,
    .track_name = track_name,
    .track_default = track_default,
    .track_s = track_s,
    .track_e = track_e,
    .check_n = check_n,
    .func_proc = func_proc,
};

ExEdit::Filter* filter_list[] = {
    &effect_ef, &filter_ef,
    NULL
};
EXTERN_C __declspec(dllexport)ExEdit::Filter** __stdcall GetFilterTableList() {
    return filter_list;
}
