#include <lexer.h>

static int get_var_name_len(const char *s)
{
	int	i;
	
	i = 0;
	if (s[i] == '?')
		return (1);
	if (ft_isdigit((unsigned char)s[i]))
		return (1);
	if (!ft_isalpha((unsigned char)s[i]) && s[i] != '_')
		return (0);
	i++;
	while (ft_isalnum((unsigned char)s[i]) || s[i] == '_')
		i++;
	return (i);
}

int	handle_dollar_sign(t_lexer *lx, const char *input)
{
	char	*num_buf;
	int		i;

    if (input[lx->i + 1] == '\0')
    {
        buf_add(lx, '$');
        lx->i++;
        return (0);
    }
    if (input[lx->i + 1] == '?')
    {
		num_buf = ft_itoa(lx->last_exit_status);
		if (!num_buf)
			return (-1);
        i = 0;
        while (num_buf[i])
			buf_add(lx, num_buf[i++]);
		lx->i += 1;
		free(num_buf);
        return (0);
    }
	return (1);
}

void buf_add_var(t_lexer *lx, const char *input)
{
	int		var_len;
	char	*var_name;
	char	*value;
	int		i;

	if (handle_dollar_sign(lx, input) <= 0)
		return ;
	var_len = get_var_name_len(input + lx->i + 1);
	if (var_len == 0)
	{
		buf_add(lx, '$');
		lx->i++;
		return;
	}
	var_name = ft_strndup(input + lx->i + 1, var_len);
	value = get_env_value(var_name, lx->env);
	i = 0;
	if (value)
	{
		while (value[i])
			buf_add(lx, value[i++]);
		free(value);
    }	
	free(var_name);
	lx->i += var_len;
}