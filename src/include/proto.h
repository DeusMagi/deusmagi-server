#ifndef __CPROTO__
#include <decls.h>
extern void world_maker(void);
/* src/commands/permission/arrest.c */
extern void command_arrest(object *op, const char *command, char *params);
/* src/commands/permission/ban.c */
extern void command_ban(object *op, const char *command, char *params);
/* src/commands/permission/config.c */
extern void command_config(object *op, const char *command, char *params);
/* src/commands/permission/follow.c */
extern void command_follow(object *op, const char *command, char *params);
/* src/commands/permission/freeze.c */
extern void command_freeze(object *op, const char *command, char *params);
/* src/commands/permission/kick.c */
extern void command_kick(object *op, const char *command, char *params);
/* src/commands/permission/memfree.c */
extern void command_memfree(object *op, const char *command, char *params);
/* src/commands/permission/memleak.c */
extern void command_memleak(object *op, const char *command, char *params);
/* src/commands/permission/mod_chat.c */
extern void command_mod_chat(object *op, const char *command, char *params);
/* src/commands/permission/no_chat.c */
extern void command_no_chat(object *op, const char *command, char *params);
/* src/commands/permission/opsay.c */
extern void command_opsay(object *op, const char *command, char *params);
/* src/commands/permission/password.c */
extern void command_password(object *op, const char *command, char *params);
/* src/commands/permission/resetmap.c */
extern void command_resetmap(object *op, const char *command, char *params);
/* src/commands/permission/resetmaps.c */
extern void command_resetmaps(object *op, const char *command, char *params);
/* src/commands/permission/server_chat.c */
extern void command_server_chat(object *op, const char *command, char *params);
/* src/commands/permission/settime.c */
extern void command_settime(object *op, const char *command, char *params);
/* src/commands/permission/shutdown.c */
extern void command_shutdown(object *op, const char *command, char *params);
/* src/commands/permission/stats.c */
extern void command_stats(object *op, const char *command, char *params);
/* src/commands/permission/tcl.c */
extern void command_tcl(object *op, const char *command, char *params);
/* src/commands/permission/tgm.c */
extern void command_tgm(object *op, const char *command, char *params);
/* src/commands/permission/tli.c */
extern void command_tli(object *op, const char *command, char *params);
/* src/commands/permission/tls.c */
extern void command_tls(object *op, const char *command, char *params);
/* src/commands/permission/tp.c */
extern void command_tp(object *op, const char *command, char *params);
/* src/commands/permission/tphere.c */
extern void command_tphere(object *op, const char *command, char *params);
/* src/commands/permission/tsi.c */
extern void command_tsi(object *op, const char *command, char *params);
/* src/commands/player/afk.c */
extern void command_afk(object *op, const char *command, char *params);
/* src/commands/player/apply.c */
extern void command_apply(object *op, const char *command, char *params);
/* src/commands/player/chat.c */
extern void command_chat(object *op, const char *command, char *params);
/* src/commands/player/drop.c */
extern void command_drop(object *op, const char *command, char *params);
/* src/commands/player/gsay.c */
extern void command_gsay(object *op, const char *command, char *params);
/* src/commands/player/hiscore.c */
extern void command_hiscore(object *op, const char *command, char *params);
/* src/commands/player/left.c */
extern void command_left(object *op, const char *command, char *params);
/* src/commands/player/me.c */
extern void command_me(object *op, const char *command, char *params);
/* src/commands/player/motd.c */
extern void command_motd(object *op, const char *command, char *params);
/* src/commands/player/my.c */
extern void command_my(object *op, const char *command, char *params);
/* src/commands/player/party.c */
extern void command_party(object *op, const char *command, char *params);
/* src/commands/player/push.c */
extern void command_push(object *op, const char *command, char *params);
/* src/commands/player/rename.c */
extern void command_rename(object *op, const char *command, char *params);
/* src/commands/player/reply.c */
extern void command_reply(object *op, const char *command, char *params);
/* src/commands/player/right.c */
extern void command_right(object *op, const char *command, char *params);
/* src/commands/player/say.c */
extern void command_say(object *op, const char *command, char *params);
/* src/commands/player/statistics.c */
extern void command_statistics(object *op, const char *command, char *params);
/* src/commands/player/take.c */
extern void command_take(object *op, const char *command, char *params);
/* src/commands/player/tell.c */
extern void command_tell(object *op, const char *command, char *params);
/* src/commands/player/time.c */
extern void command_time(object *op, const char *command, char *params);
/* src/commands/player/version.c */
extern void command_version(object *op, const char *command, char *params);
/* src/commands/player/whereami.c */
extern void command_whereami(object *op, const char *command, char *params);
/* src/commands/player/who.c */
extern void command_who(object *op, const char *command, char *params);
/* src/loaders/map_header.c */
extern int map_lex_load(mapstruct *m);
extern int map_set_variable(mapstruct *m, char *buf);
extern void free_map_header_loader(void);
extern int load_map_header(mapstruct *m, FILE *fp);
extern void save_map_header(mapstruct *m, FILE *fp, int flag);
/* src/loaders/object.c */
/* src/loaders/random_map.c */
extern int rmap_lex_read(RMParms *RP);
extern int load_parameters(FILE *fp, int bufstate, RMParms *RP);
extern int set_random_map_variable(RMParms *rp, const char *buf);
extern void free_random_map_loader(void);
/* src/random_maps/decor.c */
extern void put_decor(mapstruct *map, char **layout, RMParms *RP);
/* src/random_maps/door.c */
extern int surround_check2(char **layout, int x, int y, int Xsize, int Ysize);
extern void put_doors(mapstruct *the_map, char **maze, char *doorstyle, RMParms *RP);
/* src/random_maps/exit.c */
extern void find_in_layout(int mode, char target, int *fx, int *fy, char **layout, RMParms *RP);
extern void place_exits(mapstruct *map, char **maze, char *exitstyle, int orientation, RMParms *RP);
extern void unblock_exits(mapstruct *map, char **maze, RMParms *RP);
/* src/random_maps/expand2x.c */
extern char **expand2x(char **layout, int xsize, int ysize);
/* src/random_maps/floor.c */
extern mapstruct *make_map_floor(char *floorstyle, RMParms *RP);
/* src/random_maps/maze_gen.c */
extern char **maze_gen(int xsize, int ysize, int option);
/* src/random_maps/monster.c */
extern void place_monsters(mapstruct *map, char *monsterstyle, int difficulty, RMParms *RP);
/* src/random_maps/random_map.c */
extern void dump_layout(char **layout, RMParms *RP);
extern mapstruct *generate_random_map(char *OutFileName, RMParms *RP);
extern char **layoutgen(RMParms *RP);
extern char **symmetrize_layout(char **maze, int sym, RMParms *RP);
extern char **rotate_layout(char **maze, int rotation, RMParms *RP);
extern void roomify_layout(char **maze, RMParms *RP);
extern int can_make_wall(char **maze, int dx, int dy, int dir, RMParms *RP);
extern int make_wall(char **maze, int x, int y, int dir);
extern void doorify_layout(char **maze, RMParms *RP);
extern void write_map_parameters_to_string(char *buf, RMParms *RP);
/* src/random_maps/rogue_layout.c */
extern int surround_check(char **layout, int i, int j, int Xsize, int Ysize);
extern char **roguelike_layout_gen(int xsize, int ysize, int options);
/* src/random_maps/room_gen_onion.c */
extern char **map_gen_onion(int xsize, int ysize, int option, int layers);
extern void centered_onion(char **maze, int xsize, int ysize, int option, int layers);
extern void bottom_centered_onion(char **maze, int xsize, int ysize, int option, int layers);
extern void draw_onion(char **maze, float *xlocations, float *ylocations, int layers);
extern void make_doors(char **maze, float *xlocations, float *ylocations, int layers, int options);
extern void bottom_right_centered_onion(char **maze, int xsize, int ysize, int option, int layers);
/* src/random_maps/room_gen_spiral.c */
extern char **map_gen_spiral(int xsize, int ysize, int option);
extern void connect_spirals(int xsize, int ysize, int sym, char **layout);
/* src/random_maps/snake.c */
extern char **make_snake_layout(int xsize, int ysize);
/* src/random_maps/square_spiral.c */
extern void find_top_left_corner(char **maze, int *cx, int *cy);
extern char **make_square_spiral_layout(int xsize, int ysize);
/* src/random_maps/style.c */
extern int load_dir(const char *dir, char ***namelist, int skip_dirs);
extern mapstruct *styles;
extern mapstruct *load_style_map(char *style_name);
extern mapstruct *find_style(const char *dirname, const char *stylename, int difficulty);
extern object *pick_random_object(mapstruct *style);
extern void free_style_maps(void);
/* src/random_maps/wall.c */
extern int surround_flag(char **layout, int i, int j, RMParms *RP);
extern int surround_flag2(char **layout, int i, int j, RMParms *RP);
extern int surround_flag3(mapstruct *map, int i, int j, RMParms *RP);
extern int surround_flag4(mapstruct *map, int i, int j, RMParms *RP);
extern void make_map_walls(mapstruct *map, char **layout, char *w_style, RMParms *RP);
extern object *pick_joined_wall(object *the_wall, char **layout, int i, int j, RMParms *RP);
extern object *retrofit_joined_wall(mapstruct *the_map, int i, int j, int insert_flag, RMParms *RP);
/* src/server/account.c */
extern void account_init(void);
extern void account_deinit(void);
extern char *account_make_path(const char *name);
extern void account_login(socket_struct *ns, char *name, char *password);
extern void account_register(socket_struct *ns, char *name, char *password, char *password2);
extern void account_new_char(socket_struct *ns, char *name, char *archname);
extern void account_login_char(socket_struct *ns, char *name);
extern void account_logout_char(socket_struct *ns, player *pl);
extern void account_password_change(socket_struct *ns, char *password, char *password_new, char *password_new2);
extern void account_password_force(object *op, char *name, const char *password);
/* src/server/anim.c */
extern Animations *animations;
extern int num_animations;
extern int animations_allocated;
extern void free_all_anim(void);
extern void init_anim(void);
extern int find_animation(const char *name);
extern void animate_object(object *op);
extern void animate_turning(object *op);
/* src/server/apply.c */
extern int manual_apply(object *op, object *tmp, int aflag);
extern int player_apply(object *pl, object *op, int aflag, int quiet);
extern void player_apply_below(object *pl);
/* src/server/arch.c */
/* src/server/attack.c */
/* src/server/ban.c */
/* src/server/cache.c */
extern cache_struct *cache_find(shstr *key);
extern int cache_add(const char *key, void *ptr, uint32_t flags);
extern int cache_remove(shstr *key);
extern void cache_remove_all(void);
extern void cache_remove_by_flags(uint32_t flags);
/* src/server/commands.c */
extern void toolkit_commands_init(void);
extern void toolkit_commands_deinit(void);
extern void commands_add(const char *name, command_func handle_func, double delay, uint64_t flags);
extern int commands_check_permission(player *pl, const char *command);
extern void commands_handle(object *op, char *cmd);
/* src/server/connection.c */
extern void connection_object_add(object *op, mapstruct *map, int connected);
extern void connection_object_remove(object *op);
extern int connection_object_get_value(const object *op);
extern void connection_trigger(object *op, int state);
extern void connection_trigger_button(object *op, int state);
/* src/server/exp.c */
/* src/server/faction.c */
/* src/server/gods.c */
extern object *find_god(const char *name);
extern const char *determine_god(object *op);
/* src/server/hiscore.c */
extern void hiscore_init(void);
extern void hiscore_check(object *op, int quiet);
extern void hiscore_display(object *op, int max, const char *match);
/* src/server/image.c */
extern New_Face *new_faces;
extern int nroffiles;
extern int nrofpixmaps;
extern int read_bmap_names(void);
extern int find_face(const char *name, int error);
extern void free_all_images(void);
/* src/server/init.c */
extern struct settings_struct settings;
extern shstr_constants shstr_cons;
extern int world_darkness;
extern unsigned long todtick;
extern char first_map_path[256];
extern int first_map_x;
extern int first_map_y;
extern void free_strings(void);
extern void cleanup(void);
extern void init_globals(void);
extern void write_todclock(void);
extern void init(int argc, char **argv);
/* src/server/item.c */
extern StringBuffer *object_get_material(const object *op, const object *caller, StringBuffer *sb);
extern char *object_get_material_s(const object *op, const object *caller);
extern StringBuffer *object_get_title(const object *op, const object *caller, StringBuffer *sb);
extern char *object_get_title_s(const object *op, const object *caller);
extern StringBuffer *object_get_name(const object *op, const object *caller, StringBuffer *sb);
extern char *object_get_name_s(const object *op, const object *caller);
extern StringBuffer *object_get_short_name(const object *op, const object *caller, StringBuffer *sb);
extern char *object_get_short_name_s(const object *op, const object *caller);
extern StringBuffer *object_get_material_name(const object *op, const object *caller, StringBuffer *sb);
extern char *object_get_material_name_s(const object *op, const object *caller);
extern StringBuffer *object_get_base_name(const object *op, const object *caller, StringBuffer *sb);
extern char *object_get_base_name_s(const object *op, const object *caller);
extern StringBuffer *object_get_description_terrain(const object *op, const object *caller, StringBuffer *sb);
extern char *object_get_description_terrain_s(const object *op, const object *caller);
extern StringBuffer *object_get_description_attacks(const object *op, const object *caller, StringBuffer *sb);
extern char *object_get_description_attacks_s(const object *op, const object *caller);
extern StringBuffer *object_get_description_protections(const object *op, const object *caller, StringBuffer *sb);
extern char *object_get_description_protections_s(const object *op, const object *caller);
extern StringBuffer *object_get_description_path(const object *op, const object *caller, const uint32_t path, const char *name, StringBuffer *sb);
extern char *object_get_description_path_s(const object *op, const object *caller, const uint32_t path, const char *name);
extern StringBuffer *object_get_description(const object *op, const object *caller, StringBuffer *sb);
extern char *object_get_description_s(const object *op, const object *caller);
extern StringBuffer *object_get_name_description(const object *op, const object *caller, StringBuffer *sb);
extern char *object_get_name_description_s(const object *op, const object *caller);
extern bool need_identify(const object *op);
extern void identify(object *op);
extern void set_trapped_flag(object *op);
/* src/server/light.c */
extern void adjust_light_source(mapstruct *map, int x, int y, int light);
extern void check_light_source_list(mapstruct *map);
extern void remove_light_source_list(mapstruct *map);
/* src/server/links.c */
extern mempool_struct *pool_objectlink;
extern void objectlink_init(void);
extern void objectlink_deinit(void);
extern objectlink *get_objectlink(void);
extern void free_objectlink(objectlink *ol);
extern void free_objectlinkpt(objectlink *obp);
extern objectlink *objectlink_link(objectlink **startptr, objectlink **endptr, objectlink *afterptr, objectlink *beforeptr, objectlink *objptr);
extern objectlink *objectlink_unlink(objectlink **startptr, objectlink **endptr, objectlink *objptr);
/* src/server/living.c */
extern double dam_bonus[30 + 1];
extern int wc_bonus[30 + 1];
extern float speed_bonus[30 + 1];
extern double falling_mitigation[30 + 1];
extern uint32_t weight_limit[30 + 1];
extern int learn_spell[30 + 1];
extern int monster_signal_chance[30 + 1];
extern int savethrow[115 + 1];
extern const char *const restore_msg[5];
extern const char *const lose_msg[5];
extern const char *const statname[5];
extern const char *const short_stat_name[5];
extern void set_attr_value(living *stats, int attr, int8_t value);
extern void change_attr_value(living *stats, int attr, int8_t value);
extern int8_t get_attr_value(const living *stats, int attr);
extern void check_stat_bounds(living *stats);
extern void drain_stat(object *op);
extern void drain_specific_stat(object *op, int deplete_stats);
extern void living_update_player(object *op);
extern void living_update_monster(object *op);
extern int living_update(object *op);
extern object *living_get_base_info(object *op);
extern object *living_find_base_info(object *op);
extern void set_mobile_speed(object *op, int idx);
/* src/server/los.c */
extern void init_block(void);
extern void set_block(int x, int y, int bx, int by);
extern void update_los(object *op);
extern void clear_los(object *op);
extern int obj_in_line_of_sight(object *obj, rv_vector *rv);
/* src/server/main.c */
extern player *first_player;
extern mapstruct *first_map;
extern treasure_list_t *first_treasurelist;
extern struct artifact_list *first_artifactlist;
extern player *last_player;
extern uint32_t global_round_tag;
extern int process_delay;
extern void version(object *op);
extern void leave_map(object *op);
extern void set_map_timeout(mapstruct *map);
extern void process_events(void);
extern void clean_tmp_files(void);
extern void server_shutdown(void);
extern int swap_apartments(const char *mapold, const char *mapnew, int x, int y, object *op);
extern void shutdown_timer_start(long secs);
extern void shutdown_timer_stop(void);
extern int main(int argc, char **argv);
extern void main_process(void);
/* src/server/map.c */
/* src/server/move.c */
extern int get_random_dir(void);
extern int get_randomized_dir(int dir);
extern int object_move_to(object *op, int dir, object *originator, mapstruct *m, int x, int y);
extern int move_ob(object *op, int dir, object *originator);
extern int transfer_ob(object *op, int x, int y, int randomly, object *originator, object *trap);
extern int push_ob(object *op, int dir, object *pusher);
/* src/server/object.c */
/* src/server/object_methods.c */
/* src/server/party.c */
extern const char *const party_loot_modes[PARTY_LOOT_MAX];
extern const char *const party_loot_modes_help[PARTY_LOOT_MAX];
extern party_struct *first_party;
extern void party_init(void);
extern void party_deinit(void);
extern void add_party_member(party_struct *party, object *op);
extern void remove_party_member(party_struct *party, object *op);
extern void form_party(object *op, const char *name);
extern party_struct *find_party(const char *name);
extern int party_can_open_corpse(object *pl, object *corpse);
extern void party_handle_corpse(object *pl, object *corpse);
extern void send_party_message(party_struct *party, const char *msg, int flag, object *op, object *except);
extern void remove_party(party_struct *party);
extern void party_update_who(player *pl);
/* src/server/pathfinder.c */
extern void path_request(object *waypoint);
extern object *path_get_next_request(void);
extern shstr *path_encode(path_node_t *path);
extern int path_get_next(shstr *buf, int16_t *off, shstr **mappath, mapstruct **map, int *x, int *y, uint32_t *flags);
extern path_node_t *path_compress(path_node_t *path);
extern void path_visualize(path_visualization_t **visualization, path_visualizer_t **visualizer);
extern path_node_t *path_find(object *op, mapstruct *map1, int x, int y, mapstruct *map2, int x2, int y2, path_visualizer_t **visualizer);
/* src/server/plugins.c */
extern object *get_event_object(object *op, int event_nr);
extern void display_plugins_list(object *op);
extern void init_plugins(void);
extern void init_plugin(const char *pluginfile);
extern void remove_plugin(const char *id);
extern void remove_plugins(void);
extern void map_event_obj_init(object *ob);
extern void map_event_free(map_event *tmp);
extern int trigger_map_event(int event_id, mapstruct *m, object *activator, object *other, object *other2, const char *text, int parm);
extern void trigger_global_event(int event_type, void *parm1, void *parm2);
extern void trigger_unit_event(object *const activator, object *const me);
extern int trigger_event(int event_type, object *const activator, object *const me, object *const other, const char *msg, int parm1, int parm2, int parm3, int flags);
/* src/server/quest.c */
extern void quest_handle(object *op, object *quest);
/* src/server/race.c */
extern const char *item_races[13];
extern ob_race *race_find(shstr *name);
extern ob_race *race_get_random(void);
extern void race_init(void);
extern void race_free(void);
/* src/server/re-cmp.c */
extern const char *re_cmp(const char *str, const char *regexp);
/* src/server/readable.c */
extern int book_overflow(const char *buf1, const char *buf2, size_t booksize);
extern void init_readable(void);
extern object *get_random_mon(void);
extern void tailor_readable_ob(object *book, int msg_type);
extern void free_all_readable(void);
/* src/server/region.c */
extern region_struct *first_region;
extern void regions_init(void);
extern void regions_free(void);
extern region_struct *region_find_by_name(const char *region_name);
extern const region_struct *region_find_with_map(const region_struct *region);
extern const char *region_get_longname(const region_struct *region);
extern const char *region_get_msg(const region_struct *region);
extern int region_enter_jail(object *op);
/* src/server/rune.c */
extern int trap_see(object *op, object *trap, int level);
extern int trap_show(object *trap, object *where);
extern int trap_disarm(object *disarmer, object *trap);
extern void trap_adjust(object *trap, int difficulty);
/* src/server/shop.c */
extern int64_t shop_get_cost(object *op, int mode);
extern const char *shop_get_cost_string(int64_t cost);
extern const char *shop_get_cost_string_item(object *op, int flag);
extern int64_t shop_get_money(object *op);
extern bool shop_pay(object *op, int64_t to_pay);
extern bool shop_pay_item(object *op, object *item);
extern bool shop_pay_items(object *op);
extern void shop_sell_item(object *op, object *item);
extern int64_t bank_get_balance(object *op);
extern object *bank_find_info(object *op);
extern int bank_deposit(object *op, const char *text, int64_t *value);
extern int bank_withdraw(object *op, const char *text, int64_t *value);
extern void shop_insert_coins(object *op, int64_t value);
/* src/server/skill_util.c */
extern float stat_exp_mult[30 + 1];
extern int64_t do_skill(object *op, int dir, const char *params);
extern int64_t calc_skill_exp(object *who, object *op, int level);
extern void init_new_exp_system(void);
extern int check_skill_to_fire(object *op, object *weapon);
extern void link_player_skills(object *pl);
extern int change_skill(object *who, int sk_index);
extern int skill_attack(object *tmp, object *pl, int dir, char *string);
extern int SK_level(object *op);
extern object *SK_skill(object *op);
/* src/server/skills.c */
extern skill_struct skills[NROFSKILLS];
extern void find_traps(object *pl, int level);
extern void remove_trap(object *op);
/* src/server/spell_effect.c */
extern void cast_magic_storm(object *op, object *tmp, int lvl);
extern int recharge(object *op);
extern int cast_create_food(object *op, object *caster, int dir, const char *stringarg);
extern int cast_wor(object *op, object *caster);
extern void cast_destruction(object *op, object *caster, int dam);
extern int cast_heal_around(object *op, int level, int type);
extern int cast_heal(object *op, object *caster, int level, object *target, int spell_type);
extern int cast_change_attr(object *op, object *caster, object *target, int spell_type);
extern int cast_remove_depletion(object *op, object *target);
extern int remove_curse(object *op, object *target, int type, int src);
extern int do_cast_identify(object *tmp, object *op, int mode, int *done, int level);
extern int cast_identify(object *op, int level, object *single_ob, int mode);
extern int cast_consecrate(object *op);
extern int finger_of_death(object *op, object *target);
extern int cast_cause_disease(object *op, object *caster, int dir, struct archetype *disease_arch, int type);
extern int cast_transform_wealth(object *op);
/* src/server/spell_util.c */
extern spell_struct spells[52];
extern const char *const spellpathnames[20];
extern struct archetype *spellarch[52];
extern void init_spells(void);
extern int insert_spell_effect(const char *archname, mapstruct *m, int x, int y);
extern spell_struct *find_spell(int spelltype);
extern int cast_spell(object *op, object *caster, int dir, int type, int ability, int item, const char *stringarg);
extern int cast_create_obj(object *op, object *new_op, int dir);
extern int fire_bolt(object *op, object *caster, int dir, int type);
extern int fire_arch_from_position(object *op, object *caster, int16_t x, int16_t y, int dir, struct archetype *at, int type, object *target);
extern int cast_cone(object *op, object *caster, int dir, int strength, int spell_type, struct archetype *spell_arch);
extern void cone_drop(object *op);
extern void explode_object(object *op);
extern void check_fired_arch(object *op);
extern int find_target_for_spell(object *op, object **target, uint32_t flags);
extern int SP_level_dam_adjust(object *caster, int spell_type, bool exact);
extern int SP_level_strength_adjust(object *caster, int spell_type);
extern int SP_level_spellpoint_cost(object *caster, int spell_type, int caster_level);
extern void fire_swarm(object *op, object *caster, int dir, struct archetype *swarm_type, int spell_type, int n, int magic);
extern void spell_failure_raw_mana(object *caster, int level);
extern void spell_failure(object *caster, int level);
extern int spell_get_random(int level, int flags);
/* src/server/statistics.c */
extern void statistics_init(void);
extern void statistic_update(const char *type, object *op, int64_t i, const char *buf);
extern void statistics_player_logout(player *pl);
/* src/server/swap.c */
extern void write_map_log(void);
extern void read_map_log(void);
extern void swap_map(mapstruct *map, int force_flag);
extern void check_active_maps(void);
extern void flush_old_maps(void);
/* src/server/time.c */
extern long max_time;
extern int max_time_multiplier;
extern long pticks;
extern struct timeval last_time;
extern const char *season_name[4];
extern const char *weekdays[7];
extern const char *month_name[12];
extern const char *periodsofday[10];
extern const int periodsofday_hours[24];
extern void reset_sleep(void);
extern void sleep_delta(void);
extern void set_max_time(long t);
extern void set_max_time_multiplier(int t);
extern void get_tod(timeofday_t *tod);
extern void print_tod(object *op);
extern void time_info(object *op);
extern long seconds(void);
/* src/server/treasure.c */
extern const char *const coins[6 + 1];
extern struct archetype *coins_arch[6];
extern void treasure_init(void);
extern treasure_list_t *treasure_list_find(const char *name);
extern object *treasure_generate_single(treasure_list_t *treasure_list, int difficulty, int artifact_chance);
extern void treasure_generate(treasure_list_t *treasure_list, object *op, int difficulty, int flags);
extern void free_all_treasures(void);
extern int get_environment_level(object *op);
/* src/server/weather.c */
extern const int season_timechange[4][24];
extern void init_world_darkness(void);
extern void tick_the_clock(void);
/* src/skills/construction.c */
extern void construction_do(object *op, int dir);
/* src/skills/inscription.c */
extern int skill_inscription(object *op, const char *params);
/* src/socket/image.c */
extern int is_valid_faceset(int fsn);
extern void free_socket_images(void);
extern void read_client_images(void);
extern void socket_command_ask_face(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void face_get_data(int face, uint8_t **ptr, uint16_t *len);
/* src/socket/info.c */
extern void draw_info_send(uint8_t type, const char *name, const char *color, socket_struct *ns, const char *buf);
extern void draw_info_full(uint8_t type, const char *name, const char *color, StringBuffer *sb_capture, object *pl, const char *buf);
extern void draw_info_full_format(uint8_t type, const char *name, const char *color, StringBuffer *sb_capture, object *pl, const char *format, ...) __attribute__((format(printf, 6, 7)));
extern void draw_info_type(uint8_t type, const char *name, const char *color, object *pl, const char *buf);
extern void draw_info_type_format(uint8_t type, const char *name, const char *color, object *pl, const char *format, ...) __attribute__((format(printf, 5, 6)));
extern void draw_info(const char *color, object *pl, const char *buf);
extern void draw_info_format(const char *color, object *pl, const char *format, ...) __attribute__((format(printf, 3, 4)));
extern void draw_info_map(uint8_t type, const char *name, const char *color, mapstruct *map, int x, int y, int dist, object *op, object *op2, const char *buf);
/* src/socket/init.c */
extern Socket_Info socket_info;
extern socket_struct *init_sockets;
extern bool init_connection(socket_struct *ns);
extern void free_all_newserver(void);
extern void free_newsocket(socket_struct *ns);
extern void init_srv_files(void);
/* src/socket/item.c */
extern unsigned int query_flags(object *op);
extern void add_object_to_packet(struct packet_struct *packet, object *op, object *pl, uint8_t apply_action, uint32_t flags, int level);
extern void esrv_draw_look(object *pl);
extern void esrv_close_container(object *pl, object *op);
extern void esrv_send_inventory(object *pl, object *op);
extern void esrv_update_item(int flags, object *op);
extern void esrv_send_item(object *op);
extern void esrv_del_item(object *op);
extern object *esrv_get_ob_from_count(object *pl, tag_t count);
extern void socket_command_item_examine(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void send_quickslots(player *pl);
extern void socket_command_quickslot(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_item_apply(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_item_lock(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_item_mark(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void esrv_move_object(object *pl, tag_t to, tag_t tag, long nrof);
/* src/socket/lowlevel.c */
extern void socket_buffer_clear(socket_struct *ns);
extern void socket_buffer_write(socket_struct *ns);
extern void socket_send_packet(socket_struct *ns, struct packet_struct *packet);
/* src/socket/metaserver.c */
extern void metaserver_info_update(void);
extern void metaserver_init(void);
extern void metaserver_deinit(void);
extern void metaserver_stats(char *buf, size_t size);
/* src/socket/request.c */
extern void socket_command_setup(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_player_cmd(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_version(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_item_move(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void esrv_update_stats(player *pl);
extern void esrv_new_player(player *pl, uint32_t weight);
extern void draw_map_text_anim(object *pl, const char *color, const char *text);
extern void draw_client_map(object *pl);
extern void packet_append_map_name(struct packet_struct *packet, object *op, object *map_info);
extern void packet_append_map_music(struct packet_struct *packet, object *op, object *map_info);
extern void packet_append_map_weather(struct packet_struct *packet, object *op, object *map_info);
extern void draw_client_map2(object *pl);
extern void socket_command_quest_list(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_clear(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_move_path(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_fire(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_keepalive(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_move(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void send_target_command(player *pl);
extern void socket_command_account(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void generate_quick_name(player *pl);
extern void socket_command_target(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_talk(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_control(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
extern void socket_command_combat(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
void
socket_command_crypto(socket_struct *ns,
                      player        *pl,
                      uint8_t       *data,
                      size_t         len,
                      size_t         pos);
void
socket_command_ask_resource(socket_struct *ns,
                            player        *pl,
                            uint8_t       *data,
                            size_t         len,
                            size_t         pos);
/* src/socket/sounds.c */
extern void play_sound_player_only(player *pl, int type, const char *filename, int x, int y, int loop, int volume);
extern void play_sound_map(mapstruct *map, int type, const char *filename, int x, int y, int loop, int volume);
/* src/socket/updates.c */
extern void updates_init(void);
extern void socket_command_request_update(socket_struct *ns, player *pl, uint8_t *data, size_t len, size_t pos);
/* src/types/common/apply.c */
extern int common_object_apply(object *op, object *applier, int aflags);
extern int object_apply_item(object *op, object *applier, int aflags);
/* src/types/common/describe.c */
extern void common_object_describe(object *op, object *observer, char *buf, size_t size);
/* src/types/common/move_on.c */
extern int common_object_move_on(object *op, object *victim, object *originator, int state);
/* src/types/common/process.c */
extern int common_object_process_pre(object *op);
extern void common_object_process(object *op);
/* src/types/common/projectile.c */
extern void common_object_projectile_process(object *op);
extern object *common_object_projectile_move(object *op);
extern object *common_object_projectile_stop_missile(object *op, int reason);
extern object *common_object_projectile_stop_spell(object *op, int reason);
extern object *common_object_projectile_fire_missile(object *op, object *shooter, int dir);
extern int common_object_projectile_hit(object *op, object *victim);
extern int common_object_projectile_move_on(object *op, object *victim, object *originator, int state);
/* src/types/monster.c */
extern void set_npc_enemy(object *npc, object *enemy, rv_vector *rv);
extern void monster_enemy_signal(object *npc, object *enemy);
extern object *check_enemy(object *npc, rv_vector *rv);
extern object *find_enemy(object *npc, rv_vector *rv);
extern int talk_to_npc(object *op, object *npc, char *txt);
extern int is_friend_of(object *op, object *obj);
extern int check_good_weapon(object *who, object *item);
extern int check_good_armour(object *who, object *item);
extern _Bool monster_is_ally_of(object *op, object *target);
extern void monster_drop_arrows(object *op);
#endif
