/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_banner.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 23:31:58 by hbani-at          #+#    #+#             */
/*   Updated: 2026/06/05 00:15:45 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execution.h"

void	print_banner(void)
{
	printf("\033[2J\033[H");
	printf("\033[1;36m");
	printf("╔═╗┬ ┬┌─┐┬  ┬  ╦ ╦┌─┐╦═╗╔═╗╔═╗╦  ┬┌┬┐\n");
	printf("╚═╗├─┤├┤ │  │  ║║║├┤ ╠╦╝║╣ ╠═╝║  │ │ \n");
	printf("╚═╝┴ ┴└─┘┴─┘┴─┘╚╩╝└─┘╩╚═╚═╝╩  ╩═╝┴ ┴ \n");
	printf("\033[0m");
}

char   *readline_prompt(t_program_info *info)
 {
        char    *prompt;
        char    *user;
        char    *pwd;

        user = env_find_value(info->env, "USER");
        pwd = env_find_value(info->env, "PWD");
        if (!user)
                user = "";
        if (!pwd)
                pwd = "";
        prompt = ft_strdup(GREEN "ShellWeREPLit(" YELLOW);
        prompt = strjoin_free(prompt, user);
        prompt = strjoin_free(prompt, GREEN ") [" LIGHT_BLUE);
        prompt = strjoin_free(prompt, pwd);
        prompt = strjoin_free(prompt, GREEN "] " END_COLOR "# ");
        return (prompt);
 }
