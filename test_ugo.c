#define COMMAND 555
#define PIPE 444
#define OPERATOR 333

enum    e_type
{
    COMMAND,
    PIPE,
    OPERATOR,
}

enum    e_pipe
{
    START,
    MID,
    END,
}

enum    e_state
{
    NORMAL,
    IN_QUOTES,
    IN_DQUOTES
}

struct cmd
{
    char *command_raw; //grep -e "salut"
    char **command; //["grep"] ["-e"] ["salut"]
    int numbr_of_params;
    enum type = COMMAND;
    enum pipe;
    s_token *prev;
	s_token *next;
}

if (token.type == COMMAND)
