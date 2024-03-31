// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef QUERYOBJECT_H
#define QUERYOBJECT_H

#include "GQLSchema.h"

namespace graphql::database::object {
namespace methods::QueryHas {

template <class TImpl>
concept getGet_userWithParams = requires (TImpl impl, service::FieldParams params, std::optional<int> idArg)
{
	{ service::AwaitableObject<std::shared_ptr<User>> { impl.getGet_user(std::move(params), std::move(idArg)) } };
};

template <class TImpl>
concept getGet_user = requires (TImpl impl, std::optional<int> idArg)
{
	{ service::AwaitableObject<std::shared_ptr<User>> { impl.getGet_user(std::move(idArg)) } };
};

template <class TImpl>
concept getAll_usersWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableObject<std::optional<std::vector<std::shared_ptr<User>>>> { impl.getAll_users(std::move(params)) } };
};

template <class TImpl>
concept getAll_users = requires (TImpl impl)
{
	{ service::AwaitableObject<std::optional<std::vector<std::shared_ptr<User>>>> { impl.getAll_users() } };
};

template <class TImpl>
concept getSearchWithParams = requires (TImpl impl, service::FieldParams params, std::string term1Arg, std::string term2Arg)
{
	{ service::AwaitableObject<std::vector<std::shared_ptr<User>>> { impl.getSearch(std::move(params), std::move(term1Arg), std::move(term2Arg)) } };
};

template <class TImpl>
concept getSearch = requires (TImpl impl, std::string term1Arg, std::string term2Arg)
{
	{ service::AwaitableObject<std::vector<std::shared_ptr<User>>> { impl.getSearch(std::move(term1Arg), std::move(term2Arg)) } };
};

template <class TImpl>
concept beginSelectionSet = requires (TImpl impl, const service::SelectionSetParams params)
{
	{ impl.beginSelectionSet(params) };
};

template <class TImpl>
concept endSelectionSet = requires (TImpl impl, const service::SelectionSetParams params)
{
	{ impl.endSelectionSet(params) };
};

} // namespace methods::QueryHas

class [[nodiscard]] Query final
	: public service::Object
{
private:
	[[nodiscard]] service::AwaitableResolver resolveGet_user(service::ResolverParams&& params) const;
	[[nodiscard]] service::AwaitableResolver resolveAll_users(service::ResolverParams&& params) const;
	[[nodiscard]] service::AwaitableResolver resolveSearch(service::ResolverParams&& params) const;

	[[nodiscard]] service::AwaitableResolver resolve_typename(service::ResolverParams&& params) const;
	[[nodiscard]] service::AwaitableResolver resolve_schema(service::ResolverParams&& params) const;
	[[nodiscard]] service::AwaitableResolver resolve_type(service::ResolverParams&& params) const;

	std::shared_ptr<schema::Schema> _schema;

	struct [[nodiscard]] Concept
	{
		virtual ~Concept() = default;

		virtual void beginSelectionSet(const service::SelectionSetParams& params) const = 0;
		virtual void endSelectionSet(const service::SelectionSetParams& params) const = 0;

		[[nodiscard]] virtual service::AwaitableObject<std::shared_ptr<User>> getGet_user(service::FieldParams&& params, std::optional<int>&& idArg) const = 0;
		[[nodiscard]] virtual service::AwaitableObject<std::optional<std::vector<std::shared_ptr<User>>>> getAll_users(service::FieldParams&& params) const = 0;
		[[nodiscard]] virtual service::AwaitableObject<std::vector<std::shared_ptr<User>>> getSearch(service::FieldParams&& params, std::string&& term1Arg, std::string&& term2Arg) const = 0;
	};

	template <class T>
	struct [[nodiscard]] Model
		: Concept
	{
		Model(std::shared_ptr<T>&& pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		[[nodiscard]] service::AwaitableObject<std::shared_ptr<User>> getGet_user(service::FieldParams&& params, std::optional<int>&& idArg) const final
		{
			if constexpr (methods::QueryHas::getGet_userWithParams<T>)
			{
				return { _pimpl->getGet_user(std::move(params), std::move(idArg)) };
			}
			else
			{
				static_assert(methods::QueryHas::getGet_user<T>, R"msg(Query::getGet_user is not implemented)msg");
				return { _pimpl->getGet_user(std::move(idArg)) };
			}
		}

		[[nodiscard]] service::AwaitableObject<std::optional<std::vector<std::shared_ptr<User>>>> getAll_users(service::FieldParams&& params) const final
		{
			if constexpr (methods::QueryHas::getAll_usersWithParams<T>)
			{
				return { _pimpl->getAll_users(std::move(params)) };
			}
			else
			{
				static_assert(methods::QueryHas::getAll_users<T>, R"msg(Query::getAll_users is not implemented)msg");
				return { _pimpl->getAll_users() };
			}
		}

		[[nodiscard]] service::AwaitableObject<std::vector<std::shared_ptr<User>>> getSearch(service::FieldParams&& params, std::string&& term1Arg, std::string&& term2Arg) const final
		{
			if constexpr (methods::QueryHas::getSearchWithParams<T>)
			{
				return { _pimpl->getSearch(std::move(params), std::move(term1Arg), std::move(term2Arg)) };
			}
			else
			{
				static_assert(methods::QueryHas::getSearch<T>, R"msg(Query::getSearch is not implemented)msg");
				return { _pimpl->getSearch(std::move(term1Arg), std::move(term2Arg)) };
			}
		}

		void beginSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::QueryHas::beginSelectionSet<T>)
			{
				_pimpl->beginSelectionSet(params);
			}
		}

		void endSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::QueryHas::endSelectionSet<T>)
			{
				_pimpl->endSelectionSet(params);
			}
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	Query(std::unique_ptr<const Concept>&& pimpl) noexcept;

	[[nodiscard]] service::TypeNames getTypeNames() const noexcept;
	[[nodiscard]] service::ResolverMap getResolvers() const noexcept;

	void beginSelectionSet(const service::SelectionSetParams& params) const final;
	void endSelectionSet(const service::SelectionSetParams& params) const final;

	const std::unique_ptr<const Concept> _pimpl;

public:
	template <class T>
	Query(std::shared_ptr<T> pimpl) noexcept
		: Query { std::unique_ptr<const Concept> { std::make_unique<Model<T>>(std::move(pimpl)) } }
	{
	}

	[[nodiscard]] static constexpr std::string_view getObjectType() noexcept
	{
		return { R"gql(Query)gql" };
	}
};

} // namespace graphql::database::object

#endif // QUERYOBJECT_H
