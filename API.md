# asuna's Lua API
Lua API for `asuna` stored inside of table `asuna.` that is being created only when script is ran through Lua Executor, so it won't be accessed by anti-cheats or any other client-side files on Server(s) *(blind guess)*

`API.md` is in active development, and we not even sure, that descriptions will actually brought there, since most of functions, explains themselves in their names

## Tables
### `asuna.player_info_s` -> `table`
### `asuna.globalvars` -> `table`
### `asuna.exploits` -> `table`

## Functions
### `asuna.load_bytecode(string bytecode)` -> `any`
### `asuna.log(string message)` -> `nil`
### `asuna.get_player_info_s(number userId)` -> `player_info_s`
### `asuna.get_latency(number flow)` -> `number`
### `asuna.clientcmd(string command)` -> `nil`
### `asuna.in_screenshot()` -> `boolean`
### `asuna.change_name(string name)` -> `boolean`
### `asuna.custom_disconnect(string reason)` -> `boolean`
### `asuna.get_view_angles()` -> `table`
### `asuna.set_view_angles(number x, number y)` -> `nil`
### `asuna.is_in_game()` -> `boolean`
### `asuna.get_local_player()` -> `number`
### `asuna.load_script(string fileName)` -> `nil`
### `asuna.get_convar(string name)` -> `table`
### `asuna.set_convar(string name, number value)` -> `nil`
### `asuna.set_convar_string(string name, string value)` -> `nil`
### `asuna.exploits.sourcenet()` -> `nil`
### `asuna.exploits.achievement(number long, number achievementId)` -> `nil`
### `asuna.exploits.request_file(string fileName)` -> `nil`
### `asuna.exploits.send_file(string fileName, number transferId)` -> `nil`
### `asuna.exploits.srv_message(string message)` -> `nil`
### `asuna.exploits.clientinfo()` -> `nil`
### `asuna.exploits.listenevents()` -> `nil`