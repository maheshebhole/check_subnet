#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

int is_prefix_match (uint32_t prefix, uint8_t prefix_len, uint32_t ip_addr)
{
		uint32_t subnet_mask, nw_addr;
		
		subnet_mask = 0xFFFFFFFF << (32- prefix_len);
		nw_addr = (subnet_mask & ip_addr);
		prefix = (subnet_mask & prefix);
		if (nw_addr == prefix)
			return 1;

		return 0;
}

int parse_prefix (char *cmdline_str, struct in_addr *prefix, uint8_t *prefix_len)
{
	char prefix_str[INET_ADDRSTRLEN], ch;
	int ret, i = 0;
	uint8_t tmp = 0, digit;

	memset(prefix_str, 0, sizeof(prefix_str));
	while (cmdline_str[i] != '/') {
		prefix_str[i] = cmdline_str[i];
		i++;
	}
	prefix_str[i] = '\0';
	i++;

	while (cmdline_str[i] != '\0') {
		digit = cmdline_str[i] - 48;
		tmp *= 10;
		tmp += digit;
		i++;
	}

	*prefix_len = tmp;
	ret = inet_pton(AF_INET, prefix_str, &prefix->s_addr);
	if (ret != 1) {
		printf("\ninet_pton failed, errno = %u, ip_str = %s\n",
				errno, prefix_str);
		return -1;
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	int rc;
	struct in_addr prefix, ip_addr;
	uint8_t prefix_len;

	if (argc != 3) {
		printf("\nUsage: ./check_subnet <prefix:IPv4 prefix (IP/length)>  <IPv4 Address to check>\n");
		return 0;
	}

	memset(&prefix, 0, sizeof(prefix));
	memset(&ip_addr, 0, sizeof(ip_addr));

	rc = parse_prefix(argv[1], &prefix, &prefix_len);
	if (rc < 0) {
		return 0;
	}

	rc = inet_pton(AF_INET, argv[2], &ip_addr.s_addr);
	if (rc != 1) {
		printf("\ninet_pton failed, errno = %u, ip_str = %s\n",
			errno, argv[2]);
		return 0;
	}

	rc = is_prefix_match(ntohl(prefix.s_addr), prefix_len, ntohl(ip_addr.s_addr));
	printf("Prefix Match = %s\n", rc ? "YES" : "NO");
	return 0;
}
