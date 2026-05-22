#include "args.h"
#include "ctest.h"

CTEST(args, default_options)
{
    PwgenOptions opts;
    set_default_options(&opts);

    ASSERT_EQUAL(8, opts.length);
    ASSERT_EQUAL(1, opts.count);
    ASSERT_FALSE(opts.capitalize);
    ASSERT_FALSE(opts.no_capitalize);
    ASSERT_FALSE(opts.numerals);
    ASSERT_FALSE(opts.no_numerals);
    ASSERT_FALSE(opts.symbols);
    ASSERT_FALSE(opts.secure);
    ASSERT_FALSE(opts.no_ambiguous);
}

CTEST(args, short_flags)
{
    char* argv[] = {"pwgen", "-c", "-n", "-y", "-s", "-B"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(6, argv, &opts);
    ASSERT_EQUAL(PARSE_OK, r);
    ASSERT_TRUE(opts.capitalize);
    ASSERT_FALSE(opts.no_capitalize);
    ASSERT_TRUE(opts.numerals);
    ASSERT_FALSE(opts.no_numerals);
    ASSERT_TRUE(opts.symbols);
    ASSERT_TRUE(opts.secure);
    ASSERT_TRUE(opts.no_ambiguous);
}

CTEST(args, long_flags)
{
    char* argv[]
            = {"pwgen",
               "--capitalize",
               "--numerals",
               "--symbols",
               "--secure",
               "--no-ambiguous"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(6, argv, &opts);
    ASSERT_EQUAL(PARSE_OK, r);
    ASSERT_TRUE(opts.capitalize);
    ASSERT_TRUE(opts.numerals);
    ASSERT_TRUE(opts.symbols);
    ASSERT_TRUE(opts.secure);
    ASSERT_TRUE(opts.no_ambiguous);
}

CTEST(args, conflict_cA_last_wins)
{
    char* argv[] = {"pwgen", "-c", "-A"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(3, argv, &opts);
    ASSERT_EQUAL(PARSE_OK, r);
    ASSERT_FALSE(opts.capitalize);
    ASSERT_TRUE(opts.no_capitalize);
}

CTEST(args, conflict_Ac_last_wins)
{
    char* argv[] = {"pwgen", "-A", "-c"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(3, argv, &opts);
    ASSERT_EQUAL(PARSE_OK, r);
    ASSERT_TRUE(opts.capitalize);
    ASSERT_FALSE(opts.no_capitalize);
}

CTEST(args, conflict_n0_last_wins)
{
    char* argv[] = {"pwgen", "-n", "-0"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(3, argv, &opts);
    ASSERT_EQUAL(PARSE_OK, r);
    ASSERT_FALSE(opts.numerals);
    ASSERT_TRUE(opts.no_numerals);
}

CTEST(args, positional_length_and_count)
{
    char* argv[] = {"pwgen", "16", "5"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(3, argv, &opts);
    ASSERT_EQUAL(PARSE_OK, r);
    ASSERT_EQUAL(16, opts.length);
    ASSERT_EQUAL(5, opts.count);
}

CTEST(args, invalid_length_too_small)
{
    char* argv[] = {"pwgen", "3"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(2, argv, &opts);
    ASSERT_EQUAL(PARSE_ERROR, r);
}

CTEST(args, invalid_length_too_large)
{
    char* argv[] = {"pwgen", "257"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(2, argv, &opts);
    ASSERT_EQUAL(PARSE_ERROR, r);
}

CTEST(args, invalid_count_too_small)
{
    char* argv[] = {"pwgen", "8", "0"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(3, argv, &opts);
    ASSERT_EQUAL(PARSE_ERROR, r);
}

CTEST(args, invalid_count_too_large)
{
    char* argv[] = {"pwgen", "8", "10001"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(3, argv, &opts);
    ASSERT_EQUAL(PARSE_ERROR, r);
}

CTEST(args, help_flag)
{
    char* argv[] = {"pwgen", "-h"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(2, argv, &opts);
    ASSERT_EQUAL(PARSE_HELP, r);
}

CTEST(args, version_flag)
{
    char* argv[] = {"pwgen", "--version"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(2, argv, &opts);
    ASSERT_EQUAL(PARSE_VERSION, r);
}

CTEST(args, too_many_positional_args)
{
    char* argv[] = {"pwgen", "8", "5", "extra"};
    PwgenOptions opts;
    set_default_options(&opts);

    const ParseResult r = parse_args(4, argv, &opts);
    ASSERT_EQUAL(PARSE_ERROR, r);
}
