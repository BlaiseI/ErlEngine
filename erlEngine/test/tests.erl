-module(tests).
-include_lib("eunit/include/eunit.hrl").

initiate_test() ->
    erlEngine:sdl_init(),
    finished.