-module(erlEngine).
-on_load(init/0).
-export([sdl_init/0]).

test_fun() ->
    nif_not_loaded.

sdl_init() ->
    test_fun(),
    ok.

init() ->
    SoName = case code:priv_dir(erlEngine) of
        {error, bad_name} ->
            case filelib:is_dir(filename:join(["..", priv])) of
                true ->
                    filename:join(["..", priv, erlEngine]);
                _ ->
                    filename:join([priv, erlEngine])
            end;
        Dir ->
            filename:join(Dir, erlEngine)
    end,
    erlang:load_nif(SoName, 0).


