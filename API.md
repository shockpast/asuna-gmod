# asuna's Lua API
Lua API for `asuna` stored inside of table `asuna.` that is being created only when script is ran through Lua Executor, so it won't be accessed by anti-cheats or any other client-side files on Server(s) *(blind guess)*

`API.md` is in active development, and we not even sure, that descriptions will actually brought there, since most of functions, explains themselves in their names

## Tables
### `asuna.common` -> `table`

## Functions
### `asuna.log(string message)` -> `nil`
### `asuna.clientcmd(string command)` -> `nil`
### `asuna.change_name(string name)` -> `boolean`
### `asuna.custom_disconnect(string reason)` -> `boolean`
### `asuna.include(string fileName)` -> `nil`
### `asuna.common.random_string(number length)` -> `nil`