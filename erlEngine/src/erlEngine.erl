-module(erlEngine).
-on_load(init/0).
-export([initiate/0]).

initiate() ->
    nif_not_loaded.

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


