
#if defined(XCVAR_PROTO)
#define XCVAR_DEF(name, defVal, update, flags) extern vmCvar_t name;
#elif defined(XCVAR_DECL)
#define XCVAR_DEF(name, defVal, update, flags) vmCvar_t name;
#elif defined(XCVAR_LIST)
#define XCVAR_DEF(name, defVal, update, flags) {&name, #name, defVal, update, flags},
#else
#pragma message("missing XCVAR expansion def")
#define XCVAR_DEF(...)
#endif

XCVAR_DEF(capturelimit, "0", NULL, CVAR_ARCHIVE | CVAR_NORESTART | CVAR_SERVERINFO) // fixme init'd to 8 in game module
XCVAR_DEF(cg_drawCrosshair, "1", NULL, CVAR_ARCHIVE)
XCVAR_DEF(cg_drawCrosshairNames, "1", NULL, CVAR_ARCHIVE)
XCVAR_DEF(cg_marks, "1", NULL, CVAR_ARCHIVE)
XCVAR_DEF(cg_selectedPlayer, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(cg_selectedPlayerName, "", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(cjp_client, "1.4B4", NULL, CVAR_USERINFO | CVAR_ROM)
XCVAR_DEF(cp_clanpwd, "", NULL, CVAR_USERINFO | CVAR_ARCHIVE)
XCVAR_DEF(cp_login, "", NULL, CVAR_USERINFO)
XCVAR_DEF(cp_pluginDisable, "0", NULL, CVAR_USERINFO | CVAR_ARCHIVE)
XCVAR_DEF(cp_sbRGB1, "0", NULL, CVAR_ARCHIVE | CVAR_USERINFO)
XCVAR_DEF(cp_sbRGB2, "0", NULL, CVAR_ARCHIVE | CVAR_USERINFO)
XCVAR_DEF(g_botsFile, "", NULL, CVAR_INIT | CVAR_ROM)
XCVAR_DEF(g_spSkill, "2", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(g_warmup, "20", NULL, CVAR_ARCHIVE)
XCVAR_DEF(japp_crashHandler, "1", NULL, CVAR_ARCHIVE)
XCVAR_DEF(japp_preciseFonts, "0", NULL, CVAR_ARCHIVE)
XCVAR_DEF(japp_ratioFix, "0", UI_Set2DRatio, CVAR_ARCHIVE)
XCVAR_DEF(japp_version, JAPP_VERSION, NULL, CVAR_ROM)
XCVAR_DEF(se_language, "english", NULL, CVAR_ARCHIVE | CVAR_NORESTART)
XCVAR_DEF(ui_PrecacheModels, "0", NULL, CVAR_ARCHIVE)
XCVAR_DEF(ui_blueteam, DEFAULT_BLUETEAM_NAME, NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_blueteam1, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_blueteam2, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_blueteam3, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_blueteam4, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_blueteam5, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_blueteam6, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_blueteam7, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_blueteam8, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_browserShowEmpty, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_browserShowFull, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_browserShowPasswordProtected, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_bypassMainMenuLoad, "0", NULL, CVAR_INTERNAL)
XCVAR_DEF(ui_captureLimit, "5", NULL, CVAR_INTERNAL)
XCVAR_DEF(ui_char_anim, "BOTH_WALK1", NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_char_color_blue, "255", NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_char_color_green, "255", NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_char_color_red, "255", NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_char_model, "jedi_tf", NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_char_skin_head, "head_a1", NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_char_skin_legs, "lower_a1", NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_char_skin_torso, "torso_a1", NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_ctf_capturelimit, "8", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_ctf_friendly, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_ctf_timelimit, "30", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_currentMap, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_currentNetMap, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_currentOpponent, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_dedicated, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_ffa_fraglimit, "20", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_ffa_timelimit, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_findPlayer, "Kyle", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_forcePowerDisable, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_fragLimit, "10", NULL, CVAR_INTERNAL)
XCVAR_DEF(ui_freeSaber, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_gameType, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_joinGameType, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_lastServerRefresh_0, "", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_lastServerRefresh_1, "", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_lastServerRefresh_2, "", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_lastServerRefresh_3, "", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_lastServerRefresh_4, "", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_lastServerRefresh_5, "", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_lastServerRefresh_6, "", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_mapIndex, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_menuFilesMP, "ui/jampmenus.txt", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_modSay, "0", NULL, CVAR_NONE | CVAR_INTERNAL)
XCVAR_DEF(ui_modSayText, "", NULL, CVAR_NONE | CVAR_INTERNAL)
XCVAR_DEF(ui_modSayAllPlayer, "0", NULL, CVAR_NONE | CVAR_INTERNAL)
XCVAR_DEF(ui_netGameType, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_netSource, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_opponentName, DEFAULT_BLUETEAM_NAME, NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_rankChange, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_recordSPDemo, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_redteam, DEFAULT_REDTEAM_NAME, NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_redteam1, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL) // rww - these used to all default to 0 (closed).. I changed them to 1 (human)
XCVAR_DEF(ui_redteam2, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_redteam3, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_redteam4, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_redteam5, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_redteam6, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_redteam7, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_redteam8, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_sab1_b, "255", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_sab1_g, "255", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_sab1_r, "255", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_sab2_b, "255", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_sab2_g, "255", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_sab2_r, "255", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_saber, DEFAULT_SABER, NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_saber2, "none", NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_saber2_color, "yellow", NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_saber_color, "yellow", NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_saber_type, "single", NULL, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreAccuracy, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreAssists, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreBase, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreCaptures, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreDefends, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreExcellents, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreGauntlets, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreImpressives, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scorePerfect, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreScore, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreShutoutBonus, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreSkillBonus, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreTeam, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreTime, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_scoreTimeBonus, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_selectedModelIndex, "16", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_serverFilterType, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_serverStatusTimeOut, "7000", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_showMissingSkins, "1", NULL, CVAR_ARCHIVE)
XCVAR_DEF(ui_singlePlayerActive, "0", NULL, CVAR_INTERNAL)
XCVAR_DEF(ui_sv_master1, "", CVU_Master1, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_sv_master2, "", CVU_Master2, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_sv_master3, "", CVU_Master3, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_sv_master4, "", CVU_Master4, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_sv_master5, "", CVU_Master5, CVAR_ROM | CVAR_INTERNAL)
XCVAR_DEF(ui_team_fraglimit, "0", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_team_friendly, "1", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)
XCVAR_DEF(ui_team_timelimit, "20", NULL, CVAR_ARCHIVE | CVAR_INTERNAL)

#undef XCVAR_DEF
